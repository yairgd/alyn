-- Define 16 colors in RGB444 format (hexadecimal)
-- Define 32 colors in RGB444 format (hexadecimal)
local colors = {
    0x000, -- Black
    0xF00, -- Red
    0x0F0, -- Green
    0xFF0, -- Yellow
    0x00F, -- Blue
    0xF0F, -- Magenta
    0x0FF, -- Cyan
    0xFFF, -- White
    0x444, -- Gray
    0xA00, -- Dark Red
    0x080, -- Dark Green
    0xAA0, -- Dark Yellow
    0x008, -- Dark Blue
    0xA0A, -- Dark Magenta
    0x088, -- Dark Cyan
    0xCCC, -- Light Gray
    0x666, -- Medium Gray
    0xF88, -- Light Red
    0x0F8, -- Light Green
    0xFF8, -- Light Yellow
    0x08F, -- Light Blue
    0xF0A, -- Light Magenta
    0x0AA, -- Light Cyan
    0xFFF, -- White (full intensity)
    0xC00, -- Dark Orange
    0x8C0, -- Dark Chartreuse
    0xCC0, -- Dark Peach
    0x00C, -- Dark Purple
    0xC0C, -- Dark Lavender
    0x0CC, -- Dark Turquoise
    0xFFF, -- White (full intensity)
    0xFFF, -- White (full intensity)
}-- Accessing a color by index

local colorIndex = 3 -- Green
local color = colors[colorIndex]

-- Printing the color in hexadecimal format
print(string.format("Color %d: 0x%X", colorIndex, color))
