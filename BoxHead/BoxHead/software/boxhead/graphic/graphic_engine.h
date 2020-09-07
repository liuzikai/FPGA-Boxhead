//
// Created by liuzikai on 9/6/20.
//

#ifndef BOXHEAD_GRAPHIC_ENGINE_H
#define BOXHEAD_GRAPHIC_ENGINE_H

extern void graphic_engine_reset();

extern void draw(int start_x, int end_x, int start_y, int end_y, int src_offset, int palette, int flip_x);

extern void wait_for_next_frame();

#endif //BOXHEAD_GRAPHIC_ENGINE_H
