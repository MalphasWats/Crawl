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
    '#0000ff',
    '#ff00ff',
    '#ffff00',
    '#00ffff',
    '#f0f0f0',
    '#0f0f0f',
    '#000000',
    '#00ff00',
    '#0000ff',
    '#ff00ff',
    '#ffff00',
    '#00ffff',
    '#f0f0f0',
    '#0f0f0f',
]

SOLIDS = [1]

class Map(object):

    def __init__(self, height, width):
        self.cells = []
        self.height = height
        self.width = width

        self.signatures = []

        self.tk = Tk()
        self.tk.title("Cave Generator")

        self.canvas = Canvas(self.tk, width=self.width*16, height=self.height*16)
        self.canvas.pack()

        self.tk.bind("<KeyRelease>", self.btn)
        self.tk.bind("<Button-1>", self.show_sig)
        self.go()
        self.tk.mainloop()


    def go(self, e=None):
        self.cells = []
        for y in range(self.height):
            for x in range(self.width):
                self.cells.append( random.randint(0,1) )

        #for i in range(1):
        #    self._apply_automaton((6, 7, 8), (3, 4, 5, 6, 7, 8))

        #for i in range(1):
    #        self._apply_automaton((5, 6, 7, 8), (5, 6, 7, 8))

        self.generate_signatures()

        self.draw_nicely()

    def show_sig(self, evt):
        self.generate_signatures()
        x = evt.x//16
        y = evt.y//16
        print(x, y, format(self.signatures[x][y], '#010b'))

    def btn(self, evt):
        key = evt.keysym
        if key == 'space':
            self.go()
        if key == 'a':
            self._apply_automaton((6, 7, 8), (3, 4, 5, 6, 7, 8))
        if key == 'f':
            self.fill_holes()
        if key == 't':
            self.trim_points()
        if key == 'd':
            self.draw_doors()
        if key == 'e':
            self.kill_diags()
        if key == 'c':
            self.label_caverns()
        if key == 'w':
            self.worms()
        if key == 'r':
            self.reset_floor()
        if key == 'x':
            self.remove_small_caverns()
        if key == 's':
            self.save_cave()
        if key == 'b':
            self._apply_automaton((6, 7, 8), (3, 4, 5, 6, 7, 8))
            self.fill_holes()
            self.trim_points()
            self.kill_diags()
            self.label_caverns()
            self.worms()
            self.reset_floor()
            num_caverns = self.label_caverns()

            if (num_caverns > 1):
                self.remove_small_caverns()
            self.reset_floor()
            self.draw_doors()
            self.label_caverns()

        self.draw_nicely()

    def remove_small_caverns(self):
        sizes = [0]*6

        for y in range(self.height):
            for x in range(self.width):
                flag = self.cells[y*self.width+x]
                if flag >= 3:
                    sizes[flag-3] += 1

        biggest = 0
        flag = 0
        for i in range(6):
            if sizes[i] > biggest:
                biggest = sizes[i]
                flag = i
        flag += 3
        for y in range(self.height):
            for x in range(self.width):
                cel = self.cells[y*self.width+x]
                if cel != 1 and cel != flag:
                    self.cells[y*self.width+x] = 1


    def reset_floor(self):
        for y in range(self.height):
            for x in range(self.width):
                if self.cells[y*self.width+x] != 1:
                    self.cells[y*self.width+x] = 0

    def worms(self):
        for y in range(self.height):
            for x in range(self.width-3):
                blocks = 0
                if self.cells[y*self.width+x]==1:
                    blocks |= 1<<3
                if self.cells[y*self.width+x+1]==1:
                    blocks |= 1<<2
                if self.cells[y*self.width+x+2]==1:
                    blocks |= 1<<1
                if self.cells[y*self.width+x+3]==1:
                    blocks |= 1<<0

                #print(format(blocks, '#010b'))

                if blocks == 0b0110 or blocks == 0b0100 or blocks == 0b0010: #0b1001:
                    if self.cells[y*self.width+x] != self.cells[y*self.width+x+3]:
                        self.cells[y*self.width+x+1] = 2
                        self.cells[y*self.width+x+2] = 2
                        self.reset_floor()
                        self.label_caverns()

        for y in range(self.height-3):
            for x in range(self.width):
                blocks = 0
                if self.cells[y*self.width+x]==1:
                    blocks |= 1<<3
                if self.cells[(y+1)*self.width+x]==1:
                    blocks |= 1<<2
                if self.cells[(y+2)*self.width+x]==1:
                    blocks |= 1<<1
                if self.cells[(y+3)*self.width+x]==1:
                    blocks |= 1<<0

                #print(format(blocks, '#010b'))

                if blocks == 0b0110 or blocks == 0b0100 or blocks == 0b0010: #0b1001:
                    if self.cells[y*self.width+x] != self.cells[(y+3)*self.width+x]:
                        self.cells[(y+1)*self.width+x] = 2
                        self.cells[(y+2)*self.width+x] = 2
                        self.reset_floor()
                        self.label_caverns()

        self.generate_signatures()




    def kill_diags(self):
        for y in range(self.height-1):
            for x in range(self.width-1):
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
        self.generate_signatures()
        for y in range(self.height):
            for x in range(self.width):
                if (self.cells[(y*self.width)+x] == 0):
                    noexit = True
                    for i in range(4):
                        if ((self.signatures[x][y]^255) & CARDINAL_SIGNATURES[i]) == CARDINAL_SIGNATURES[i]:
                            noexit = False
                    if noexit:
                        self.cells[(y*self.width)+x] = 1


    def trim_points(self):
        self.generate_signatures()
        for y in range(self.height):
            for x in range(self.width):
                if (self.cells[(y*self.width)+x] != 0):
                    cardinal_exits = 0
                    for i in range(4):
                        if ((self.signatures[x][y]^255) & CARDINAL_SIGNATURES[i]) == CARDINAL_SIGNATURES[i]:
                            cardinal_exits += 1
                    if cardinal_exits >= 3:
                        self.cells[(y*self.width)+x] = 0


    def label_caverns(self):
        flag = 3
        for y in range(self.height):
            for x in range(self.width):
                if self.cells[y*self.width+x] == 0: #Floor
                    self.set_flags(x, y, flag)
                    flag += 1

        return flag-3

    def set_flags(self, x, y, flag):
        candidates = []
        candidates.append((x, y))
        while True:
            new_candidates = []
            for c in candidates:

                self.cells[c[1]*self.width+c[0]] = flag

                if self.cells[ (c[1]-1)*self.width+c[0]] in [0]:#, 2]:
                    if (c[0], c[1]-1) not in new_candidates:
                      new_candidates.append((c[0], c[1]-1))
                if self.cells[ (c[1]+1)*self.width+c[0]] in [0]:#, 2]:
                    if (c[0], c[1]+1) not in new_candidates:
                        new_candidates.append((c[0], c[1]+1))
                if self.cells[ c[1]*self.width+(c[0]+1)] in [0]:#, 2]:
                    if (c[0]+1, c[1]) not in new_candidates:
                        new_candidates.append((c[0]+1, c[1]))
                if self.cells[ c[1]*self.width+(c[0]-1)] in [0]:#, 2]:
                    if (c[0]-1, c[1]) not in new_candidates:
                        new_candidates.append((c[0]-1, c[1]))
            candidates = new_candidates
            if len(candidates) == 0:
                break


    def draw_doors(self):
        self.generate_signatures()
        #doors = []
        for y in range(self.height):
            for x in range(self.width):
                if ((self.signatures[x][y] & 0b01000010)==0b01000010 and
                    (self.signatures[x][y] & 0b00011000) == 0 and
                    self.cells[y*self.width+x] == 0):
                    if self.cells[ y*self.width+(x-1) ] != 2 and self.cells[ y*self.width+(x+1) ] != 2 and self.cells[ y*self.width+(x-2) ] != 2 and self.cells[ y*self.width+(x+2) ] != 2:
                        self.cells[(y*self.width)+x] = 2
                        #doors.append((x, y))

                if ((self.signatures[x][y] & 0b01000010)==0 and
                    (self.signatures[x][y] & 0b00011000) == 0b00011000 and
                    self.cells[y*self.width+x] == 0):
                    if self.cells[ (y-1)*self.width+x ] != 2 and self.cells[ (y+1)*self.width+x ] != 2 and self.cells[ (y-2)*self.width+x ] != 2 and self.cells[ (y+2)*self.width+x ] != 2:
                        self.cells[(y*self.width)+x] = 2
                        #doors.append((x, y))

        #return doors

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

    def save_cave(self):
        with open('cave.txt', 'w') as f:
            cells = self.cells
            while len(cells) > 0:
                row = cells[0:32]
                cells = cells[32:]
                for c in row:
                    f.write("{:>3}, ".format(c))
                f.write('\n')



if __name__ == '__main__':
    height = 16
    width = 32

    print("""
    Welcome to MapGen

    Instructions: <space> generates a random Map
                  Press 'B' to run through a full generation sequence

                  Or:
                    'a': run Cellular Automator
                    'f': fill in small gaps
                    't': trim pointy bits
                    'e': remove diagonal gaps
                    'c': identify caverns
                    'w': worm connections between caverns
                    'r': reset floor
                    'x': remove all but biggest cavern (must use 'c' first)
                    'd': place doors (must use 'r' first)
                    's': save cavern to a file
    """)

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
