#include <gmsh.h>
#include <blaze/Blaze.h>
#include <iostream>
#include <fstream>
#include <vector>

void readGmsh(const std::string filename, blaze::DynamicMatrix<double>* gcoord, blaze::DynamicMatrix<size_t>* nodes, size_t dim)
{  

    // Add file exists check, since gmsh will nto do that and just provides a
  // empty mesh
  std::ifstream f(filename);
  if (!f.good()) {
    std::cerr << "Error: Could not open the file: " << filename << "!"
              << std::endl;
    exit(1);
  }
  f.close();

  gmsh::initialize();
  gmsh::option::setNumber("General.Terminal", 1);
  gmsh::open(filename);

  // clear data
  //nodes->clear();
  //enc->clear();
  //nec->clear();
  //volumes->clear();

  // getting all nodes using GMSH API
  std::vector<std::size_t> nodeTags;
  std::vector<double> nodeCoords, nodeParams;
  gmsh::model::mesh::getNodes(nodeTags, nodeCoords, nodeParams, -1, -1);

  // getting all elements using GMSH API
  std::vector<int> elemTypes;
  std::vector<std::vector<std::size_t>> elemTags, elemNodeTags;
  gmsh::model::mesh::getElements(elemTypes, elemTags, elemNodeTags, -1, -1);

  // specify type of element to read
  if (dim != 2 and dim != 3) {
    std::cerr << "Error: MshReader currently only supports reading of "
                 "triangle/quadrangle elements in dimension 2 and tetragonal "
                 "elements in 3.\n";
    exit(1);
  }

gcoord->resize(nodeTags.size(),dim);

for (size_t i = 0; i < nodeTags.size(); i++) {
    size_t index = (nodeTags[i] - 1) * 3;
    (gcoord)[nodeTags[i] - 1] = {
        nodeCoords[index], nodeCoords[index + 1], nodeCoords[index + 2]};
  }

}







int main(void){

    blaze::DynamicMatrix<double> gcoord;
    blaze::DynamicMatrix<size_t> nodes;

    readGmsh("../../untitled.msh", &gcoord, &nodes, 2);

        
    return EXIT_SUCCESS;

}
