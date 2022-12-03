# Pathfinding

In this project we develop Djikstra's algorithm and A* algorithm as well as the traveling salesman algorithm (not finished) in unreal using C++.

<div style="display:flex">
<img src="https://user-images.githubusercontent.com/89453098/205418060-41e4c41d-e4aa-4e14-ae0b-fefbcda2dc54.png" style="width:49%">
<img src="https://user-images.githubusercontent.com/89453098/205418067-3cad781b-ef94-41b2-9e21-e4ce1db37218.png" style="width:49%">
</div>

### Djikstra/A*: Order of operations
<ol>
    <li>Having received a starting node, check each connected node.</li>
    <li>Add each connected node to a priority list, ordered by their current cost from the origin node.</li>
    <li>Jump to the node with the least travel cost, aka the first item in the priority list.</li>
    <li>Restart at instruction 1.</li>
</ol>

Both dijkstra and A* allows you to jump between nodes regardless of them being connected or not. You simply
jump to the node that is at the top of the priority queue.
When calculating the cost of a node, you need to include the cost not only from the current node to the node 
we are checking, but also from the current node back to origin. However this becomes simple by setting the
node's cost variable from the beginning. This way it "automatically" includes the cost back to origin.
`NextNode->Cost = CurrentNode->Cost + Distance(CurrentNode, NextNode)` 

The only difference between Dijkstra and A* is that A* also calculates the distance between the next node and
the destination.<br>
`NextNode->Cost = CurrentNode->Cost + Distance(CurrentNode, NextNode) + Distance(NextNode, Destination`<br>
This makes the priority list "smarter" by prioritizing the nodes that go in the destination's general direction.
This will often get us to the goal faster. A* immediately stops when it has made contact with the destination.

There are some known bugs, for example the debug boundary box will often glitch, which will be fixed by spawning
enough nodes. I suspect this is a bug in unreal engine with rendering debug shapes.

Developed with Unreal Engine 5
