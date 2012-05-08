from dynamic_graph.sot.curves import Nurb, Interpolator
from dynamic_graph import plug
inter = Interpolator('inter')

inter.dimension.value = 3
inter.stamp.value = (0.0, 0.2, 0.5 , 0.7, 1.0)
inter.v0.value = (0.0, 0.0, 0.0)
inter.vn.value = (0.0, 0.0, 0.0)
inter.qs.value = (0.0, 0.0, 0.0,
                  3.0, 4.0, 0.0,
                  -1.0, 4.0, 0.0,
                  -4.0, 0.0, 0.0,
                  -4.0, -3.0, 0.0,
                  )


n = Nurb('n')

plug(inter.knots, n.knots)
plug(inter.cv, n.cv)

n.degree.value = 3


for i,t in enumerate(range(11)):
    n.t.value = t/10.0
    n.t.time = i
    inter.knots.recompute(0)
    n.state.recompute(i)


    print n.t.value, n.state.value

