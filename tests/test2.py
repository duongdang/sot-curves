from dynamic_graph.sot.curves import Bezier, Timeline, Spline
from dynamic_graph import plug

bezier = Bezier('bezier')
timeline = Timeline('timeline')
spline = Spline('spline')

bezier.controlPoints.value = ((0,0), (10,20))
plug(spline.out, bezier.t)

plug(timeline.scaledTime, spline.input)
timeline.timeMax.value = 100


from dynamic_graph.sot.dynamics.hrp2 import Hrp2Laas, Hrp2Jrl
from dynamic_graph.sot.dynamics.solver import Solver
robot = Hrp2Laas("robot")
solver = Solver(robot)

robot.device.before.addSignal('timeline.dummy')
robot.device.before.addSignal('bezier.state')

robot.device.zmp.value = (0.,0.,0.)

s = ['left-ankle', 'right-ankle']
for i in s:
    robot.dynamic.signal(i).recompute(robot.dynamic.signal(i).time + 1)
    robot.features[i].reference.value =         robot.dynamic.signal(i).value
    robot.features[i]._feature.selec.value = '111111'
    robot.tasks[i].controlGain.value = 180.

    robot.featureComDes.errorIN.value = robot.dynamic.com.value
    robot.featureComDes.selec.value = '111'
    robot.comTask.controlGain.value = 180.


# Push com and feet tasks.
#
# The robot is currently in half-sitting, so this script freezes com
# and feet position so that the robot will remain stable while the
# user program is starting.
solver.push(robot.comTask.name)

for i in s:
    solver.push(robot.tasks[i].name)

timeline.setState('start')
#for i in range(1000):
while True:

    robot.device.increment(0.005)


    print robot.device.state.time, timeline.scaledTime.value, timeline.scaledTime.time, bezier.state.value
