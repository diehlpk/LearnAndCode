Mesh.Algorithm = 8; // Delaunay for quads

//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(4) = {-1, 0, 0, 1.0};
//+
Point(5) = {0, -1, 0, 1.0};
//+
Point(7) = {0, 1, 0, 1.0};
//+
Point(8) = {0.86602540378, 0.5, 0, 1.0};
//+
Point(9) = {0.86602540378, -0.5, 0, 1.0};
//+
Circle(1) = {8, 1, 7};
//+
Circle(2) = {7, 1, 4};
//+
Circle(3) = {5, 1, 4};
//+
Circle(4) = {9, 1, 5};
//+
Line(5) = {9, 1};
//+
Line(6) = {1, 8};
//+
Physical Curve(7) = {4, 3, 2, 1, 6, 5};
//+
Curve Loop(1) = {1, 2, -3, -4, 5, 6};
//+
Plane Surface(1) = {1};

Physical Surface(1) = {1};
