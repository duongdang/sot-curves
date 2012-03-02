from dynamic_graph.sot.curves import Bezier, Timeline

b = Bezier('b')
tl = Timeline('tl')

b.controlPoints.value = ((0,0),(0,1),(1,1))
tl.timeMax.value = 1
tl.setState('start')

for i,t in enumerate(range(11)):
    b.t.value = t/10.0
    b.t.time = i
    b.state.recompute(i)
    tl.dummy.recompute(i)


    print t, b.state.value, tl.scaledTime.value, tl.scaledTime.time
