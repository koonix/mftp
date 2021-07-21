#ifndef TUI_H
#define TUI_H

void tui_init();
void tui_end();
void tui_disable();
void cursor_up();
void cursor_down();
void clear_to_top();
void clear_to_bottom();
void clear_all();
void clear_line();
void cursor_home();
void cursor_line_column(int line, int column);
void tui_block_input();
void tui_no_block_input();

#endif /* TUI_H */
