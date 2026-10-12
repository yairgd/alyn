// Package sim is the alyn MCU C code (alyn common/, built by native/) linked into this
// process. C threads run the game and the tick; Lua's console hooks feed a pipe that this
// package turns into Output/Prompt messages. Nothing here touches widgets: results go to
// post, which is the app's PostInterrupt.
package sim

/*
#cgo CFLAGS: -I${SRCDIR}/../../native/src
#cgo LDFLAGS: -L${SRCDIR}/../../lib -Wl,--start-group -lalyn_sim -llua -llua_module -llua_binaries -lcommon_static -lluaEffects_static -leffects_static -lHal -Wl,--end-group -lstdc++ -lpthread -lm
#include <stdlib.h>
#include "sim.h"
*/
import "C"

import (
	"encoding/binary"
	"io"
	"os"
	"runtime"
	"time"
	"unsafe"
)

const (
	MatrixW  = int(C.SIM_W)
	MatrixH  = int(C.SIM_H_)
	Stations = int(C.SIM_STATIONS)

	frameInterval = 25 * time.Millisecond
)

// Station is one of the 8 station LEDs.
type Station struct {
	R, G, B              uint8
	Connected, Blink, On bool
}

// Frame is the merged 64x32 canvas (RGB, row-major) plus the station LEDs.
type Frame struct {
	RGB      []byte
	Stations [Stations]Station
}

// Output is Lua console text (print, errors, game errors); Prompt is the REPL prompt.
type Output struct{ Text string }
type Prompt struct{ Text string }

// Init starts the C side (tick thread, console pipe) and the frame ticker. Call once.
func Init(post func(any)) {
	C.sim_init()
	go frames(post)
	go console(post)
}

func frames(post func(any)) {
	buf := make([]byte, C.SIM_FRAME_LEN)
	for range time.Tick(frameInterval) {
		C.sim_frame((*C.uchar)(unsafe.Pointer(&buf[0])))
		f := Frame{RGB: append([]byte(nil), buf[:MatrixW*MatrixH*3]...)}
		st := buf[MatrixW*MatrixH*3:]
		for i := range f.Stations {
			b := st[i*4 : i*4+4]
			f.Stations[i] = Station{R: b[0], G: b[1], B: b[2],
				Connected: b[3]&C.ST_CONNECTED != 0, Blink: b[3]&C.ST_BLINK != 0, On: b[3]&C.ST_BLINK_ON != 0}
		}
		post(f)
	}
}

// console reads the chunks the hooks in console.c write: kind u8 | len u32 LE | bytes.
func console(post func(any)) {
	r := os.NewFile(uintptr(C.console_out_fd()), "lua-console")
	var hdr [5]byte
	for {
		if _, err := io.ReadFull(r, hdr[:]); err != nil {
			return
		}
		p := make([]byte, binary.LittleEndian.Uint32(hdr[1:]))
		if _, err := io.ReadFull(r, p); err != nil {
			return
		}
		if hdr[0] == C.CON_PROMPT {
			post(Prompt{string(p)})
		} else {
			post(Output{string(p)})
		}
	}
}

// RunConsole runs the Lua REPL (lua_main) on its own OS thread and returns when it ends
// (EOF). Start it with go; call again for a fresh Lua state.
func RunConsole() {
	runtime.LockOSThread()
	defer runtime.UnlockOSThread()
	C.console_run()
}

// SendLine answers the REPL's readline.
func SendLine(line string) {
	c := C.CString(line)
	defer C.free(unsafe.Pointer(c))
	C.console_send_line(c)
}

// ConsoleEOF ends the REPL (Ctrl-D).
func ConsoleEOF() { C.console_eof() }

func Games() []string {
	n := int(C.sim_game_count())
	out := make([]string, n)
	for i := range out {
		out[i] = C.GoString(C.sim_game_name(C.int(i)))
	}
	return out
}

func Start(game int) bool { return C.sim_game_start(C.int(game)) == 0 }
func Stop()               { C.sim_game_stop() }
func Running() bool       { return C.sim_game_running() != 0 }

// Load runs Lua source as the game (the MCU's "game in memory").
func Load(src string) bool {
	c := C.CString(src)
	defer C.free(unsafe.Pointer(c))
	return C.sim_game_start_source(c, C.size_t(len(src))) == 0
}

func Key(id int, long bool) {
	l := 0
	if long {
		l = 1
	}
	C.sim_key(C.int(id), C.int(l))
}

func Button(station int) { C.sim_button(C.int(station)) }

func Connect(station int, on bool) {
	v := 0
	if on {
		v = 1
	}
	C.sim_connect(C.int(station), C.int(v))
}
