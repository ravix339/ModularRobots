from logn import LogNReconfig

BLOCKSIZE = 8 #number of 4x4 modules in a block

analyzer = LogNReconfig(BLOCKSIZE)

# highLevelStruct = [
#     [1,1,1,1],
#     [1,1,1,1],
#     [0,0,0,0],
#     [0,0,0,0]]

# highLevelStruct = [
#     [1,1,1,1],
#     [1,0,0,1],
#     [1,0,0,1],
#     [0,0,0,0]]

# highLevelStruct = [
#     [0,0,0,0,0,0,0,0],
#     [0,0,0,1,0,0,0,0],
#     [0,0,1,1,1,0,0,0],
#     [0,0,0,0,1,0,0,0],
#     [0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0]]

highLevelStruct = [
    [0,0,0,0,1,1,1,0],
    [0,0,0,0,1,0,0,0],
    [0,0,1,1,1,0,0,0],
    [0,0,0,1,0,0,0,0],
    [0,0,0,1,0,0,0,0],
    [0,0,0,1,1,0,0,0],
    [0,0,0,1,1,0,0,0],
    [0,0,0,0,0,0,0,0]]

fullForm = analyzer.block_32_to_basic(highLevelStruct)

desiredForm, progress = analyzer.change_to_desired_form(fullForm)

for timestamp in range(len(progress)):
    analyzer.display(progress[timestamp])
    print('\n')
    out_formatted = analyzer.convert_to_out(progress[timestamp])
    f = open(str(timestamp) + '.txt',mode='w')
    f.writelines([row + '\n' for row in out_formatted])
