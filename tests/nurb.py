from dynamic_graph.sot.curves import Nurb

n = Nurb('n')

n.cv.value = (0,0,0,
              1,2,0,
              4,3,0,
              6,0,0,
              )

# n + 1 = 4 control points, n = 3
n.knots.value = (0,0,0,0,1,1,1,1)
# p = 3
# opennurbs ignore the first and the last value
n.degree.value = 3


# should be a bezier curve
for i,t in enumerate(range(11)):
    tt = max(n.knots.value)*t/10.0
    n.t.value = tt
    n.t.time = i
    n.state.recompute(i)

    print n.t.value, n.state.value

