import os
f = open('Carlsen.pgn')
data_path = "/home/aparna/quarter1/ai/chess-bot/games/"

data = f.read()
items = data.split('\r\n\r\n')
items.remove(items[len(items)-1])

i=0
while i<len(items):
    reward = 0
    headers = items[i].split('\r\n')
    result = headers[6].split()[1][1:-2]
    if result == '1/2-1/2':
        reward = 0
    elif result == '0-1':
        reward = -1
    else:
        reward = 1
    print reward

    fw = open('temp.pgn', 'w')
    fw.write(items[i])
    fw.write('\n\n')
    fw.write(items[i+1])
    fw.close()

    os.system("wine pgn2fen temp.pgn > file1")
    
    read_fen = open("file1")
    fen_moves = read_fen.readlines()
    plies = len(fen_moves)
    print plies
    
    f_res = open(data_path+str(i), 'w')
    
    f_res.write(str(reward))
    f_res.write('\n')
    f_res.write(str(plies))
    f_res.write('\n')
    for move in fen_moves:
        f_res.write(move[:-2])
        f_res.write('\n')
    i += 2
