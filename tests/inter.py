from dynamic_graph.sot.curves import Nurb, Interpolator
from dynamic_graph import plug
inter = Interpolator('inter')

inter.dimension.value = 3
inter.stamp.value = (0.0, 1.0, 3.0 , 7.0, 10.)
inter.v0.value = (0.0, 0.0, 0.0)
inter.vn.value = (0.0, 0.0, 0.0)
inter.qs.value = (1.0, 2.0, 3.0,
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
    n.t.value = t/10.0*max(inter.stamp.value)
    n.t.time = i
    inter.knots.recompute(0)
    n.state.recompute(i)

    print n.t.value, n.state.value

