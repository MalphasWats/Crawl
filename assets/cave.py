import random
from tkinter import *

SIGNATURES = [
    0b10000000, #TL
    0b01000000, #TM
    0b00100000, #TR
    0b00010000, #ML
    0b00001000, #MR
    0b00000100, #BL
    0b00000010, #BM
    0b00000001, #BR
    0b00000000,
]

CARDINAL_SIGNATURES = [
    0b01000000, #TM
    0b00010000, #ML
    0b00001000, #MR
    0b00000010, #BM
]

COLOURS = [
    '#000000',
    '#5D1800',
    '#ff0000',
    '#00ff00',
    '#0000ff'
]

SOLIDS = [1]

class Map(object):

    def __init__(self, height, width):
        self.cells = []
        self.height = height
        self.width = width

        self.signatures = []

        self.tk = Tk()

        self.canvas = Canvas(self.tk, width=self.width*16, height=self.height*16)
        self.canvas.pack()

        self.tk.bind("<KeyRelease>", self.btn)
        self.tk.bind("<Button-1>", self.show_sig)
        self.go()
        self.tk.mainloop()


    def go(self, e=None):
        self.cells = []
        for y in range(height):
            for x in range(width):
                self.cells.append( random.randint(0,1) )

        for i in range(1):
            self._apply_automaton((6, 7, 8), (3, 4, 5, 6, 7, 8))

        #for i in range(1):
    #        self._apply_automaton((5, 6, 7, 8), (5, 6, 7, 8))

        self.generate_signatures()

        self.draw_nicely()

    def show_sig(self, evt):
        x = evt.x//16
        y = evt.y//16
        print(x, y, format(self.signatures[x][y], '#010b'))

    def btn(self, evt):
        key = evt.keysym
        if key == 'space':
            self.go()
        if key == 'f':
            self.fill_holes()
        if key == 't':
            self.trim_points()
        if key == 'd':
            self.draw_doors()
        if key == 'e':
            self.kill_diags()

        self.draw_nicely()

    def kill_diags(self):
        for y in range(height-1):
            for x in range(width-1):
                blocks = 0
                blocks |= self.cells[y*self.width+x] << 0
                blocks |= self.cells[y*self.width+x+1] << 1
                blocks |= self.cells[(y+1)*self.width+x] << 2
                blocks |= self.cells[(y+1)*self.width+x+1] << 3

                if blocks == 0b0110: #0b1001:
                    self.cells[y*self.width+x] = 1
                    self.cells[(y+1)*self.width+x+1] = 1
                elif blocks == 0b1001: #0b0110:
                    self.cells[y*self.width+x+1] = 1
                    self.cells[(y+1)*self.width+x] = 1
        self.generate_signatures()


    def fill_holes(self):
        for y in range(height):
            for x in range(width):
                if (self.cells[(y*self.width)+x] == 0):
                    noexit = True
                    for i in range(4):
                        if ((self.signatures[x][y]^255) & CARDINAL_SIGNATURES[i]) == CARDINAL_SIGNATURES[i]:
                            noexit = False
                    if noexit:
                        self.cells[(y*self.width)+x] = 1
        self.generate_signatures()

    def trim_points(self):
        for y in range(height):
            for x in range(width):
                if (self.cells[(y*self.width)+x] != 0):
                    cardinal_exits = 0
                    for i in range(4):
                        if ((self.signatures[x][y]^255) & CARDINAL_SIGNATURES[i]) == CARDINAL_SIGNATURES[i]:
                            cardinal_exits += 1
                    if cardinal_exits >= 3:
                        self.cells[(y*self.width)+x] = 0
        self.generate_signatures()

    def draw_doors(self):
        for y in range(height):
            for x in range(width):
                if ((( (self.signatures[x][y] & 0b01000010)==0b01000010 and (self.signatures[x][y] & 0b00011000) == 0) or
                   ( (self.signatures[x][y] & 0b01000010)==0 and (self.signatures[x][y] & 0b00011000) == 0b00011000)) and
                   self.cells[(y*self.width)+x] in SOLIDS):
                    self.cells[(y*self.width)+x] = 2

    def generate_signatures(self):
        self.signatures = [ [0]*self.height for _ in range(self.width) ]

        for y in range(self.height):
            for x in range(self.width):
                if x == 0 or y == 0 or y == self.height-1 or x == self.width-1:
                    sig = 255
                else:
                    sig = 0
                    if self.cells[ ((y-1)*self.width)+(x-1) ] in SOLIDS:
                        sig |= SIGNATURES[0]
                    if self.cells[ ((y-1)*self.width)+x ] in SOLIDS:
                        sig |= SIGNATURES[1]
                    if self.cells[ ((y-1)*self.width)+(x+1) ] in SOLIDS:
                        sig |= SIGNATURES[2]
                    if self.cells[ (y*self.width)+(x-1) ] in SOLIDS:
                        sig |= SIGNATURES[3]
                    if self.cells[ (y*self.width)+(x+1) ] in SOLIDS:
                        sig |= SIGNATURES[4]
                    if self.cells[ ((y+1)*self.width)+(x-1) ] in SOLIDS:
                        sig |= SIGNATURES[5]
                    if self.cells[ ((y+1)*self.width)+x ] in SOLIDS:
                        sig |= SIGNATURES[6]
                    if self.cells[ ((y+1)*self.width)+(x+1) ] in SOLIDS:
                        sig |= SIGNATURES[7]
                #print(format(sig, '#010b'))
                self.signatures[x][y] = sig

    def _apply_automaton(self, born, survive):
        new_cells = []
        for y in range(self.height):
            for x in range(self.width):
                if x == 0 or y == 0 or y == self.height-1 or x == self.width-1:
                    new_cells.append(True)
                else:
                    neighbours = 0

                    neighbours += self.cells[ ((y-1)*self.width)+(x-1) ]
                    neighbours += self.cells[ ((y-1)*self.width)+x ]
                    neighbours += self.cells[ ((y-1)*self.width)+(x+1) ]

                    neighbours += self.cells[ (y*self.width)+(x-1) ]
                    neighbours += self.cells[ (y*self.width)+(x+1) ]

                    neighbours += self.cells[ ((y+1)*self.width)+(x-1) ]
                    neighbours += self.cells[ ((y+1)*self.width)+x ]
                    neighbours += self.cells[ ((y+1)*self.width)+(x+1) ]

                    if self.cells[ (y*self.width)+x ]:
                        new_cells.append(neighbours in survive)
                    else:
                        new_cells.append(neighbours in born)

        self.cells = new_cells

    def collide_with_wall(self, x, y):
        return self.cells[ y*self.width+x ]

    def draw(self):
        print(self.height, self.width)
        for y in range(self.height):
            row = ''
            for x in range(self.width):
                if self.cells[y*self.width+x]:
                    row += '#'
                else:
                    row += ' '
            print(row)

    def draw_nicely(self):
        self.canvas.delete("all")
        for y in range(self.height):
            for x in range(self.width):
                cel = self.cells[ (y*self.width)+x ]
                if cel != 0:
                    self.canvas.create_rectangle(x*16, y*16, (x*16)+16, (y*16)+16, fill=COLOURS[cel], outline='')


        #self.canvas.after(100, self.draw_nicely)



if __name__ == '__main__':
    height = 16
    width = 32

    #tk = Tk()

    #canvas = Canvas(tk, width=width*16, height=height*16)
    #canvas.pack()

    map = Map(height, width)
    #map.draw()

    #for y in range(height):
    #    for x in range(width):
    #        if map.cells[ (y*width)+x ]:
    #            canvas.create_rectangle(x*16, y*16, (x*16)+16, (y*16)+16, fill='#5D1800', outline='')

    #map.go()

    #map.tk.mainloop()
