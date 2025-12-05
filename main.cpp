#include <iostream>

namespace paint {
  struct p_t {
    int x,y;
  };
  bool operator==(p_t, p_t);
  bool operator!=(p_t, p_t);
  struct f_t {
    p_t aa, bb;
  };

  struct IDraw {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
    virtual ~IDraw() = default;
  };

  struct Dot: IDraw {
    Dot(int x, int y);
    explicit Dot(p_t dd);
    p_t d;
    p_t begin() const override;
    p_t next(p_t) const override;
  };
  
  struct VerticalLine: IDraw {
    VerticalLine(p_t s, p_t e);
    p_t start,end;
    p_t begin() const override;
    p_t next(p_t) const override;
  };

  //Расширить заданный массив точками из очередной фигуры
  //-extend...
  size_t points(const IDraw& d, p_t** pts, size_t& s);
  //Найти минимум и максимум по каждой координате среди точек и сформировать фрейм
  f_t frame(const p_t* pts, size_t s);
  //Построить полотно (из фрейма получить количество столбцов и колонок)
  char * canvas(f_t fr, char fill);
  //Координаты точки перевести в координаты в двумерном массиве
  void paintPts(char * cnv, f_t fr, p_t p, char fill);
  //Вывод двумерного массива на основе размеров определяемых фреймом
  void flush(std::ostream& os, const char* cnv, f_t fr);
}

int main() {
  using paint::IDraw;
  using paint::Dot;
  using paint::f_t;
  using paint::p_t;
  int err = 0;
  IDraw* shps[3] = {};
  p_t * pts = nullptr;
  size_t s = 0;
  try{
    shps[0] = new Dot(0,0);
    shps[1] = new Dot(5,7);
    shps[2] = new Dot(-5,-2);
    for (size_t i = 0; i<3; ++i){
      s += points(*(shps[i]),&pts ,s);
    }
    f_t fr = frame(pts, s);
    char * cnv = canvas(fr, '.');
    for (size_t i = 0; i<s; ++i){
      paintPts(cnv, fr, pts[i], '#');
    }
    delete[] cnv;
  }
  catch(...){
    err = 2;
    std::cerr << "Bad drawing\n";
  }
  delete shps[0];
  delete shps[1];
  delete shps[2];
}

paint::Dot::Dot(p_t dd):
  IDraw(),
  d{dd}
{}

paint::Dot::Dot(int x, int y):
  IDraw(),
  d{x,y}
{}

paint::VerticalLine::VerticalLine(p_t s, p_t e):
  IDraw(),
  start{s},
  end{e}
{}

paint::p_t paint::Dot::begin() const{
  return d;
}

paint::p_t paint::Dot::next(p_t prev) const{
  if (prev!=begin()){
    throw std::logic_error("Bad impl");
  }
  return d;
}

paint::p_t paint::VerticalLine::begin() const{
  return start;
}

paint::p_t paint::VerticalLine::next(p_t prev) const{
  if (prev == end){
    return begin();
  }
  else{
    return p_t{prev.x,prev.y+1};
  }
}

bool paint::operator==(p_t a, p_t b){
  return a.x == b.x && a.y == b.y;
}

bool paint::operator!=(p_t a, p_t b){
  return !(a==b);
}

