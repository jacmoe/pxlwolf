/*# This file is part of the
# ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗  
# ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝  
# ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║     
# ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
#   project
#
#   https://github.com/jacmoe/pxlwolf
#
#   (c) 2020 Jacob Moena
#
#   BSD 2-Clause "Simplified" License
#*/
#include <cstdio>
#include <cstdlib>

#include "map.hpp"
#include "dbg.hpp"
#include "geometry.hpp"

Map *CreateEmptyMap() {
    Map *map = (Map*)malloc(sizeof(struct Map));

    map->walls = NULL;
    map->numwalls = 0;

    return map;
}


Map *M_Load(const char *path) {
    FILE *f = fopen(path, "r");

    Map *map = CreateEmptyMap();

    Segment seg;
    for (int i = 0;
            fscanf(f, "%lf %lf %lf %lf",
                &seg.start.x, &seg.start.y, &seg.end.x, &seg.end.y) != EOF;
            i++)
    {
        map->walls = (Wall*)realloc(map->walls, ++(map->numwalls) * sizeof(Wall));
        Wall wall; wall.seg = seg; wall.seen = 0;
        map->walls[i] = wall;
    }

    fclose(f);

    return map;
}
