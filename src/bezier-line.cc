#include <sot/curves/bezier-line.h>


namespace ml = maal::boost;

namespace{
  void getLine(const ml::Matrix& m, int line_no,
               ml::Vector& res)
  {
    if (line_no < 0 || line_no > (int)m.nbRows())
      {
        return;
      }
    for (unsigned j = 0; j < m.nbCols(); j++)
      {
        res(j) = m.elementAt(line_no, j);
      }

  }
}

namespace dynamicgraph {
  namespace sot {
    namespace curves {

      void compute_bezier(ml::Matrix points, const float& t, ml::Vector& result){
        result.resize(points.nbCols());

        // Recursive formula
        if (points.nbRows() == 1)
          getLine(points, 0, result);
        return;

        ml::Vector B_0_n1, B_1_n;
        ml::Matrix points_0_n1 = points.extract(0,0, (int)points.nbRows() - 1,
                                                (int)points.nbCols(),points_0_n1);
        ml::Matrix points_1_n = points.extract(1,0,
                                               (int)points.nbRows() - 1,
                                               (int)points.nbCols());
        compute_bezier(points_0_n1, t, B_0_n1);
        compute_bezier(points_1_n , t, B_1_n);
        result = (1 - t)*B_0_n1 + t*B_1_n;
        return;
      }

    }
  }
}
