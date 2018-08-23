#TODO
* Tests to see if the aggregator throws should be in an aggregator test. The VMeModel unit test shouldn't need a FakeMinimizer.
* Abstract factory or factory method for obstacles.
* FIXME: ObstacleContainer takes pointer and makes unique_ptr which violates convention that ownership isn't passed if plain points are given.
* FIXME: ObstacleContainer and TargetContainer should both derive from a container interface.
* FIXME: Investigate simple_linux_js.py technique.
* FIXME: Model shouldn't know about targets and x^ref. Both of these should be the responsibility of the path planner. NmpcModel should have a set xref() method.
* Test mechanism for setting and computing tracking errors in VMeModel.
* Mike's commoent on strange behaviour switching waypoints is solved by stringing waypoints together in path planner via xref.
* Homogenize the interface of TargetContainer and ObstacleContainer
* Define what() for all custom exceptions.
* Remove usages of VMeModel::set_v, now that the seed contains it.
* Write a test for VMeModel that checks for pythagorean relationship in ex, ey, x and y when the x/yref is perpandicular to the model trajectory.
* FIXME (BUG): segfault when CLI:clear all while en route to target.