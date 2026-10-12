// Package ui holds alyn's panes.
package ui

import (
	"strings"
	"unicode/utf8"

	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"
)

const consoleScrollback = 8000

// luaPrompts are the Lua REPL prompts (lua.c).
var luaPrompts = termforge.PromptPrefixes{
	Strip: []string{"> ", ">> "},
}

// ConsoleWidget is the Lua console: an xterm view with local echo and no PTY. The REPL
// (running in the C side, see internal/sim) asks for a line with a prompt; Enter answers it.
// Same shape as gdbforge's Lua console pane. The REPL echoes nothing, so edits rewrite the
// whole input line (termforge.RewritePromptInput).
type ConsoleWidget struct {
	termforge.BaseWidget
	term *termforge.CompositeTerminal
	clip termforge.TerminalClipboard

	prompt string
	live   bool // the prompt is on screen and a line is wanted

	history   []string
	histIndex int
	histDraft string

	onLine func(string)
	onEOF  func()
}

func NewConsoleWidget(onLine func(string), onEOF func()) *ConsoleWidget {
	t := termforge.NewCompositeTerminalWithPrefix(80, 24, consoleScrollback, "")
	t.SetPromptPrefixes(luaPrompts)
	w := &ConsoleWidget{
		BaseWidget: termforge.BaseWidget{PaneName: "Lua"},
		term:       t,
		prompt:     "> ",
		onLine:     onLine,
		onEOF:      onEOF,
	}
	if ctl := t.Controller(); ctl != nil {
		ctl.SetInputHandler(func(b []byte) error { return ctl.Write(b) }) // local echo
	}
	return w
}

// Write shows REPL output (print, errors).
func (w *ConsoleWidget) Write(text string) {
	if text == "" {
		return
	}
	atBottom := w.term.AtBottom()
	w.term.WriteRaw(termforge.TerminalNewlines(text))
	w.live = false
	if atBottom {
		w.term.ScrollToBottom()
	}
}

// Note shows a message from the app (not the REPL) on its own lines, keeping the
// prompt on screen if one was waiting.
func (w *ConsoleWidget) Note(text string) {
	live := w.live
	if termforge.CurrentLine(w.term.Controller()) != "" {
		w.term.WriteRaw("\r\n")
	}
	w.Write(text)
	if live {
		w.term.WriteRaw(w.prompt)
		w.live = true
	}
}

// SetPrompt shows the prompt the REPL is waiting at.
func (w *ConsoleWidget) SetPrompt(p string) {
	w.prompt = p
	if termforge.CurrentLine(w.term.Controller()) != "" {
		w.term.WriteRaw("\r\n")
	}
	w.term.WriteRaw(p)
	w.live = true
	w.term.ScrollToBottom()
}

func (w *ConsoleWidget) SetClipboard(io termforge.ClipboardIO) {
	w.clip.Set(io)
	w.term.SetClipboard(io)
}

func (w *ConsoleWidget) SetMouseOrigin(screenX, screenY int) { w.term.SetMouseOrigin(screenX, screenY) }

func (w *ConsoleWidget) Draw(c termforge.Canvas) { w.term.Paint(c, w.Focused()) }

func (w *ConsoleWidget) HandleEvent(ev tcell.Event) {
	switch e := ev.(type) {
	case *tcell.EventMouse:
		w.term.HandleMouse(e)
	case *tcell.EventClipboard:
		w.term.PasteBytes(e.Data())
	case *tcell.EventKey:
		w.handleKey(e)
	}
}

func (w *ConsoleWidget) HandleFocusKey(ev *tcell.EventKey) bool { return w.handleKey(ev) }

func (w *ConsoleWidget) Close() { w.term.Close() }

func (w *ConsoleWidget) ctl() *termforge.TerminalController { return w.term.Controller() }

func (w *ConsoleWidget) handleKey(ev *tcell.EventKey) bool {
	switch ev.Key() {
	case tcell.KeyEnter, tcell.KeyCtrlM, tcell.KeyCtrlJ:
		if !w.term.AtBottom() {
			w.term.ScrollToBottom()
		} else {
			w.submit()
		}
		return true
	case tcell.KeyUp:
		w.historyPrev()
		return true
	case tcell.KeyDown:
		w.historyNext()
		return true
	case tcell.KeyBackspace, tcell.KeyBackspace2:
		w.edit(func(text string, cur int) (string, int) {
			if cur <= 0 {
				return text, cur
			}
			_, n := utf8.DecodeLastRuneInString(text[:cur])
			return text[:cur-n] + text[cur:], cur - n
		})
		return true
	case tcell.KeyDelete:
		w.edit(func(text string, cur int) (string, int) {
			if cur >= len(text) {
				return text, cur
			}
			_, n := utf8.DecodeRuneInString(text[cur:])
			return text[:cur] + text[cur+n:], cur
		})
		return true
	case tcell.KeyHome, tcell.KeyCtrlA:
		termforge.MovePromptCursor(w.ctl(), w.prompt, 0)
		return true
	case tcell.KeyEnd, tcell.KeyCtrlE:
		text, _ := termforge.PromptInputState(w.ctl(), w.prompt)
		termforge.MovePromptCursor(w.ctl(), w.prompt, len(text))
		return true
	case tcell.KeyCtrlU:
		w.edit(func(text string, cur int) (string, int) { return text[cur:], 0 })
		return true
	case tcell.KeyCtrlD:
		if w.onEOF != nil {
			w.onEOF()
		}
		return true
	case tcell.KeyRune:
		if ev.Modifiers()&(tcell.ModCtrl|tcell.ModAlt) == 0 {
			w.insert(ev.Rune())
			return true
		}
	}
	return w.term.HandleKey(ev)
}

// edit applies f to the input text and caret, then rewrites the line.
func (w *ConsoleWidget) edit(f func(text string, cur int) (string, int)) {
	if !w.live {
		return
	}
	text, cur := termforge.PromptInputState(w.ctl(), w.prompt)
	if cur < 0 || cur > len(text) {
		return
	}
	text, cur = f(text, cur)
	termforge.RewritePromptInput(w.ctl(), w.prompt, text)
	termforge.MovePromptCursor(w.ctl(), w.prompt, cur)
}

func (w *ConsoleWidget) insert(r rune) {
	if !w.live {
		return
	}
	text, cur := termforge.PromptInputState(w.ctl(), w.prompt)
	if cur >= len(text) {
		_ = w.ctl().SendInput([]byte(string(r))) // append rides the echo path
		return
	}
	s := string(r)
	termforge.RewritePromptInput(w.ctl(), w.prompt, text[:cur]+s+text[cur:])
	termforge.MovePromptCursor(w.ctl(), w.prompt, cur+len(s))
}

func (w *ConsoleWidget) submit() {
	if !w.live {
		return
	}
	line := ""
	if !termforge.OnEmptyPromptLine(w.ctl(), w.prompt) {
		line = strings.TrimRight(termforge.FullInputLineText(w.ctl(), w.prompt), "\r\n")
	}
	w.term.WriteRaw("\r\n")
	w.live = false
	if line != "" && (len(w.history) == 0 || w.history[len(w.history)-1] != line) {
		w.history = append(w.history, line)
	}
	w.histIndex, w.histDraft = len(w.history), ""
	if w.onLine != nil {
		w.onLine(line)
	}
}

func (w *ConsoleWidget) historyPrev() {
	if !w.live || len(w.history) == 0 || w.histIndex == 0 {
		return
	}
	if w.histIndex == len(w.history) {
		w.histDraft = termforge.FullInputLineText(w.ctl(), w.prompt)
	}
	w.histIndex--
	termforge.RewritePromptInput(w.ctl(), w.prompt, w.history[w.histIndex])
}

func (w *ConsoleWidget) historyNext() {
	if !w.live || w.histIndex >= len(w.history) {
		return
	}
	w.histIndex++
	text := w.histDraft
	if w.histIndex < len(w.history) {
		text = w.history[w.histIndex]
	}
	termforge.RewritePromptInput(w.ctl(), w.prompt, text)
}
