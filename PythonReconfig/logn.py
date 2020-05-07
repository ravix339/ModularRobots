from typing import List

class LogNReconfig: 
    #
    #
    # Public Methods
    #
    #

    def __init__(self, blocksize = 8):
        self.BLOCKSIZE = blocksize

    #Main function to call to get states
    def change_to_desired_form(self, structure : List[List[int]]) -> (List[List[int]], List[List[List[int]]]):
        _, _, progressions = self.__changeToDesiredForm_helper(structure)
        return (progressions[-1], progressions)

    def block_32_to_basic(self, lst : List[List[int]]) -> List[List[int]]:
        n = len(lst) * self.BLOCKSIZE
        ret = [[0 for _ in range(n)] for _ in range(n)]
        for row in range(len(lst)):
            for col in range(len(lst[0])):
                if lst[row][col] == 1:
                    for i in range(self.BLOCKSIZE):
                        for j in range(self.BLOCKSIZE):
                            ret[row*self.BLOCKSIZE+i][col*self.BLOCKSIZE+j] = 1
        return ret

    def display(self, lst : List[List[int]]) -> None :
        for row in self.convert_to_out(lst):
            print(row)
    
    def convert_to_out(self, lst : List[List[int]]) -> List[str]:
        n = len(lst)
        ret = [[False for _ in range(n)] for _ in range(n)]
        for row in range(n):
            for col in range(n):
                ret[row][col] = '0' if lst[row][col] == 1 else '.'
        return ["".join(row) for row in ret]


    #
    #
    # Helper Functions
    #
    #

    #Helper that deals with recursive calls
    def __changeToDesiredForm_helper(self, structure : List[List[int]], depth=0) -> (int, bool, List[List[List[int]]]):
        n = len(structure)
        if n == 2*self.BLOCKSIZE: #base case (2x2 blocks)
            nw, ne, sw, se = 0,0,0,0
            for row in range(n):
                for col in range(n):
                    if row < n//2 and col < n//2: #north west
                        nw += structure[row][col]
                    elif row < n//2 and col >= n//2: #north east
                        ne += structure[row][col]
                    elif row >= n//2 and col < n//2: #south west
                        sw += structure[row][col]
                    elif row >= n//2 and col >= n//2: #south east
                        se += structure[row][col]
            total = nw + ne + sw + se 
            if total >= self.BLOCKSIZE*self.BLOCKSIZE:
                filled = self.__fill(total, n)
                return (total, True, [structure, filled])
            return (total, False, [structure, structure])

        og_nw = [row[:n//2] for row in structure[:n//2]]
        og_ne = [row[n//2:] for row in structure[:n//2]]
        og_sw = [row[:n//2] for row in structure[n//2:]]
        og_se = [row[n//2:] for row in structure[n//2:]]

        nw_count, nw_ring, nw_pieces = self.__changeToDesiredForm_helper(og_nw, depth+1)
        ne_count, ne_ring, ne_pieces = self.__changeToDesiredForm_helper(og_ne, depth+1)
        sw_count, sw_ring, sw_pieces = self.__changeToDesiredForm_helper(og_sw, depth+1)
        se_count, se_ring, se_pieces = self.__changeToDesiredForm_helper(og_se, depth+1)

        nw = nw_pieces[-1]
        ne = ne_pieces[-1]
        sw = sw_pieces[-1]
        se = se_pieces[-1]

        progressions = []
        for time_stamp in range(len(nw_pieces)):
            progressions.append(self.__merge(nw_pieces[time_stamp], ne_pieces[time_stamp], sw_pieces[time_stamp], se_pieces[time_stamp]))

        retStructure = progressions[-1]
        
        total = nw_count + ne_count + sw_count + se_count
        num_rings = (1 if nw_ring else 0) + (1 if ne_ring else 0) + (1 if sw_ring else 0) + (1 if se_ring else 0)

        #we can form a ring (Lemma 1)
        if self.__has_backbone(og_nw) or self.__has_backbone(og_ne) or self.__has_backbone(og_sw) or self.__has_backbone(og_se): 
            filled = self.__fill(total,n)
            progressions.append(filled)
            return (total, True, progressions)
        
        north = [og_nw[i] + og_ne[i] for i in range(len(og_nw))]
        south = [og_sw[i] + og_se[i] for i in range(len(og_sw))]
        west = [og_nw[i][:n//2] for i in range(len(og_nw))] + [og_sw[i][:n//2] for i in range(len(og_sw))]
        east = [og_ne[i][:n//2] for i in range(len(og_ne))] + [og_se[i][:n//2] for i in range(len(og_se))]

        #contrapositive of Lemma 2
        if self.__has_vbackbone_middle(north, [i for i in range(len(south[0])) if south[0][i]==1]) or \
            self.__has_vbackbone_middle(list(reversed(south)), [i for i in range(len(north[-1])) if north[-1][i]==1]):
            filled = self.__fill(total,n)
            progressions.append(filled)
            return (total, True, progressions)

        #contrapositive of Lemma 2 but for horizontal
        if self.__has_hbackbone_middle(west, [i for i in range(len(east[0])) if east[0][i]==1]) or \
            self.__has_hbackbone_middle(list(reversed(east)), [i for i in range(len(west[-1])) if west[-1][i]==1]):
            filled = self.__fill(total,n)
            progressions.append(filled)
            return (total, True, progressions)

        #Lemma 6
        if num_rings > 3:
            filled = self.__fill(total,n)
            progressions.append(filled)
            return (total, True, progressions)

        #Lemma 7
        if num_rings == 2:
            filled = self.__fill(total,n)
            progressions.append(filled)
            return (total, True, progressions)

        #Lemma 8
        if num_rings == 1:
            if total >= (4*n - 4): #ring can be formed
                filled = self.__fill(total,n)
                progressions.append(filled)
                return (total, True, progressions)
            else:  #full ring cannot be formed
                ringdir = 0
                if ne_ring: ringdir = 1
                if sw_ring: ringdir = 2
                if se_ring: ringdir = 3
                #We need to keep track of where the ring was so that we can ensure that 
                filled = self.__form_partial_ring(ringdir, total,n)
                progressions.append(filled)
                return (total, False, progressions)
        
        #Lemma 9 - Redistribute excess to borders
        filled = self.__redistribute_materials(retStructure)
        progressions.append(filled)
        return (total, False, progressions)

    #
    # Fill Functionality
    #

    #Fill a structure with a full ring
    def __fill(self, total : int, n : int) -> List[List[int]]:
        retStructure = []
        retStructure.append([1 for _ in range(n)])
        for _ in range(n-2):
            retStructure.append([1] + [0 for _ in range(n-2)] + [1])
        retStructure.append([1 for _ in range(n)])
        remaining = total - (4*n-4)
        row = n-2
        col = 1
        while remaining != 0:
            retStructure[row][col] = 1
            remaining -= 1
            col += 1
            if col == n-1:
                row -= 1
                col = 1
        return retStructure

    #Form partial ring out of a single existing ring
    def __form_partial_ring(self, ringdir : int, total : int, n : int) -> List[List[int]]:
        retStructure = [[0 for _ in range(n)] for _ in range(n)]
        remaining = total
        #Form the ring that was a part of the module that existed (keep connectivity)
        if ringdir == 0: #NW
            for i in range(n//2):
                retStructure[0][i] = 1
                retStructure[i][0] = 1
                remaining -= 2
        elif ringdir == 1: #NE
            for i in range(n//2):
                retStructure[i][n-1] = 1
                retStructure[n//2+i][i] = 1
                remaining -= 2
        elif ringdir == 2: #SW
            for i in range(n//2):
                retStructure[n-1][i] = 1
                retStructure[0][n//2+i] = 1
                remaining -= 2
        elif ringdir == 3: #SE
            for i in range(n//2):
                retStructure[n//2+i][n-1]
                retStructure[n-1][n//2+i]
                remaining -= 2

        #Fill in the other sides
        for i in range(n):
            if retStructure[0][i] == 0:
                retStructure[0][i] = 1
                remaining -= 1
            if remaining == 0:
                break
        if remaining == 0:
            return retStructure
        for i in range(1, n):
            if retStructure[i][n-1] == 0:
                retStructure[i][n-1] = 1
                remaining -= 1
            if remaining == 0:
                break
        if remaining == 0:
            return retStructure    
        for i in range(n-1,-1,-1):
            if retStructure[n-1][i] == 0:
                retStructure[n-1][i] = 1
                remaining -= 1
            if remaining == 0:
                break
        if remaining == 0:
            return retStructure
        for i in range(n-2, 0, -1):
            if retStructure[n-1][i] == 0:
                retStructure[n-1][i] = 1
                remaining -= 1
            if remaining == 0:
                break
        return retStructure

    #spread materials to stick with the necessary pivots
    def __redistribute_materials(self, structure : List[List[int]]) -> List[List[int]]:
        n = len(structure)
        copy = [[0 for _ in range(n)] for _ in range(n)] #Although we don't necessary care that we're overwriting structure, it's nice not to.
        interior_count = 0
        for row in range(1,n-1):
            for col in range(1, n-1):
                if structure[row][col] == 1:
                    interior_count += 1
        
        #copy the borders to establish the necessary connections
        for col in range(n):
            copy[0][col] = structure[0][col]
            copy[n-1][col] = structure[n-1][col]
        for row in range(n):
            copy[row][0] = structure[row][0]
            copy[row][n-1] = structure[row][n-1]

        if interior_count == 0:
            return copy

        #top row
        found_pivot = False
        pos = -1
        #fill to the right
        for col in range(n):
            if copy[0][col] == 1:
                if not found_pivot:
                    pos = col
                found_pivot = True
            elif found_pivot:
                copy[0][col] = 1
                interior_count -= 1
            if interior_count == 0:
                return copy

        #go to the left of the pivot and fill, dont need to check if its 1 since we start before the first one
        for col in range(pos-1, -1, -1):
            copy[0][col] = 1
            interior_count -= 1
            if interior_count == 0:
                return copy

        #bottom row
        found_pivot = False
        pos = -1
        #fill to the right
        for col in range(n):
            if copy[n-1][col] == 1:
                if not found_pivot:
                    pos = col
                found_pivot = True
            elif found_pivot:
                copy[n-1][col] = 1
                interior_count -= 1
            if interior_count == 0:
                return copy

        #go to the left of the pivot and fill
        for col in range(pos-1, -1, -1):
            copy[n-1][col] = 1
            interior_count -= 1
            if interior_count == 0:
                return copy

        #left col
        found_pivot = False
        pos = -1
        #Fill downwards
        for row in range(n):
            if copy[row][0] == 1:
                if not found_pivot:
                    pos = row
                found_pivot = True
            elif found_pivot:
                copy[row][0] = 1
                interior_count -= 1
            if interior_count == 0:
                return copy
        #fill upwards from first pivot
        for row in range(pos-1, -1, -1):
            copy[row][0] = 1
            interior_count -= 1
            if interior_count == 0:
                return copy

        #right col
        found_pivot = False
        pos = -1
        #Fill downwards
        for row in range(n):
            if copy[row][n-1] == 1:
                if not found_pivot:
                    pos = row
                found_pivot = True
            elif found_pivot:
                copy[row][n-1] = 1
                interior_count -= 1
            if interior_count == 0:
                return copy
        #fill upwards from first pivot
        for row in range(pos-1, -1, -1):
            copy[row][n-1] = 1
            interior_count -= 1
            if interior_count == 0:
                return copy

        return copy

    #
    # Backbone Functionality
    #

    #Determine if there is a backbone in a structure
    def __has_backbone(self, structure : List[List[int]]) -> bool:
        return (len(self.__backbone_endpoints_vertical(structure)) != 0 or len(self.__backbone_endpoints_horizontal(structure)) != 0) 

    #Check if backbone from north/south to the center axis
    def __has_vbackbone_middle(self, structure : List[List[int]], connections = List[int]) -> bool:
        endpoints = self.__backbone_endpoints_vertical(structure)
        for endpoint in endpoints: 
            if endpoint in connections:
                return True
        return False

    #Check if backbone from east/west to center axis
    def __has_hbackbone_middle(self, structure : List[List[int]], connections = List[int]) -> bool:
        endpoints = self.__backbone_endpoints_horizontal(structure)
        for endpoint in endpoints: 
            if endpoint in connections:
                return True
        return False

    #Helper for vertical backbone endpoints
    def __backbone_endpoints_vertical(self, structure : List[List[int]]) -> List[int]:
        cur_indices = [i for i in range(len(structure[0])) if structure[0][i] == 1]
        new_indices = []
        for row in range(1, len(structure)):
            for col_num in range(len(cur_indices)):
                col = cur_indices[col_num]
                if structure[row][col] == 1:
                    new_indices.append(col)
                    if col_num == 0:
                        for i in range(col-1, -1, -1):
                            if structure[row][i] == 1:
                                new_indices.append(i)
                            else:
                                break
                    cap = len(structure[row])
                    if col_num != len(cur_indices)-1:
                        cap = cur_indices[col_num+1]

                    for i in range(col+1, cap):
                        if structure[row][i] == 1:
                            new_indices.append(i)
                        else:
                            break
            cur_indices = sorted(new_indices)
            new_indices = []
        return cur_indices

    #Helper for horizontal backbone endpoints
    def __backbone_endpoints_horizontal(self, structure : List[List[int]]) -> List[int]:
        cur_indices = [i for i in range(len(structure)) if structure[i][0] == 1]
        new_indices = []
        for col in range(1, len(structure[0])):
            for row_num in range(len(cur_indices)):
                row = cur_indices[row_num]
                if structure[row][col] == 1:
                    new_indices.append(row)
                    if row_num == 0:
                        for i in range(row-1, -1, -1):
                            if structure[i][col] == 1:
                                new_indices.append(i)
                            else:
                                break
                    cap = len(structure)
                    if row_num != len(cur_indices) -1:
                        cap = cur_indices[row_num+1]
                    for i in range(row+1, cap):
                        if structure[i][col] == 1:
                            new_indices.append(i)
                        else:
                            break
            cur_indices = sorted(new_indices)
            new_indices = []
        return cur_indices

    #
    # Merge Functionality
    #

    #Merge quadrants into a singular list
    def __merge(self, nw, ne, sw, se):
        n = len(nw) * 2
        retStructure = [[0 for _ in range(n)] for _ in range(n)]
        for row in range(n):
            for column in range(n):
                if row < n//2 and column < n//2: #north west
                    retStructure[row][column] = nw[row][column]
                elif row < n//2 and column >= n//2: #north east
                    retStructure[row][column] = ne[row][column - n//2]
                elif row >= n//2 and column < n//2: #south west
                    retStructure[row][column] = sw[row - n//2][column]
                elif row >= n//2 and column >= n//2: #south east
                    retStructure[row][column] = se[row - n//2][column - n//2]
        return retStructure