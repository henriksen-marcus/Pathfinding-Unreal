# Pathfinding

In this project we develop Djikstra's algorithm and A* algorithm as well as the traveling salesman algorithm in unreal C++.

### Djikstra: Order of operations
<ol>
    <li>Having received a starting node, check each connected node.</li>
    <li>Sort each connected node in an array based on their travel cost.</li>
    <li>Move to the node with the least travel cost.</li>
    <li>Now we restart the process, but this time we need to add the travel distance from all previous nodes.</li>
</ol>

Developed with Unreal Engine 5
