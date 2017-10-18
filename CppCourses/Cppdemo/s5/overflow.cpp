#include <iostream.h>
/* Returns number of characters successfully written to win. */
extern int write_to_window (window* win, char* text, int length);

class windowbuf : public streambuf {
    window* win;
  public:
    windowbuf (window* w) { win = w; }
    int sync ();
    int overflow (int ch);
};

int windowbuf::sync ()
{ streamsize n = pptr () - pbase ();
  return (n && write_to_window (win, pbase (), n) != n) ? EOF : 0;
}

int windowbuf::overflow (int ch)
{ streamsize n = pptr () - pbase ();
  if (n && sync ())
    return EOF;
  if (ch != EOF)
    {
      char cbuf[1];
      cbuf[0] = ch;
      if (write_to_window (win, cbuf, 1) != 1)
        return EOF;
    }
  pbump (-n);  // Reset pptr().
  return 0;
}


int
main (int argc, char**argv)
{
  window *win = ...;
  windowbuf wbuf(win);
  ostream wstr(&wbuf);
  wstr << "Hello world!\n";
}


