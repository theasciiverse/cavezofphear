#!/usr/bin/env python3
"""Plans the catalog preview route through a CAVEZ of PHEAR cave: walks
to the nearest diamond through dirt, again and again, letting stones and
diamonds drop into dug-out cells, and never steps down under one that will
fall. Prints the count and the moves (Up, Down, Left, Right).
Usage: plan-route.py <level file> <diamonds>"""
import sys, collections
d=bytearray(open(sys.argv[1],'rb').read()); W,H=80,23
g=[[d[y*W+x] for x in range(W)] for y in range(H)]
EMPTY,DIRT,STONE,DIA,WALL,MONEY,PLAYER=0,1,2,3,4,5,9
py,px=[(y,x) for y in range(H) for x in range(W) if g[y][x]==PLAYER][0]
g[py][px]=EMPTY
def settle():
    moved=True
    while moved:
        moved=False
        for y in range(H-2,-1,-1):
            for x in range(W):
                if g[y][x] in (STONE,DIA,MONEY) and g[y+1][x]==EMPTY and (y+1,x)!=(py,px):
                    g[y+1][x]=g[y][x]; g[y][x]=EMPTY; moved=True
def safe(y,x,ny,nx):
    # never step under a stone/diamond column that will drop into the cell we leave
    if ny==y+1 and g[y-1][x] in (STONE,DIA,MONEY): return False
    # never stand under a hanging stone with empty space we created... keep simple
    return g[ny][nx] in (EMPTY,DIRT,DIA,MONEY)
keys=[]; got=0; limit=int(sys.argv[2])
dirs={'Up':(-1,0),'Down':(1,0),'Left':(0,-1),'Right':(0,1)}
while got<limit:
    q=collections.deque([(py,px)]); prev={(py,px):None}; tgt=None
    while q:
        y,x=q.popleft()
        if g[y][x]==DIA and (y,x)!=(py,px): tgt=(y,x); break
        for k,(dy,dx) in dirs.items():
            ny,nx=y+dy,x+dx
            if 0<=ny<H and 0<=nx<W and (ny,nx) not in prev and safe(y,x,ny,nx):
                prev[(ny,nx)]=((y,x),k); q.append((ny,nx))
    if not tgt: break
    path=[]; c=tgt
    while prev[c]: c,k=prev[c][0],prev[c][1]; path.append(k)
    for k in reversed(path):
        dy,dx=dirs[k]; ny,nx=py+dy,px+dx
        if not safe(py,px,ny,nx): break
        if g[ny][nx]==DIA: got+=1
        g[ny][nx]=EMPTY; py,px=ny,nx; keys.append(k); settle()
print(got, len(keys)); print(' '.join(keys))
