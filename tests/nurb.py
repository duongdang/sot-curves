from dynamic_graph.sot.curves import Nurb

n = Nurb('n')

n.cv.value = (0,0,0,
              1,2,0,
              4,3,0,
              6,0,0
              )
n.knots.value = (0,0,0,1,1,1)


for i,t in enumerate(range(11)):
    n.t.value = t/10.0
    n.t.time = i
    n.state.recompute(i)


    print n.t.value, n.state.value
