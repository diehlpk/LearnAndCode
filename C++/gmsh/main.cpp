#include <gmsh.h>
#include <blaze/Blaze.h>
#include <iostream>
#include <fstream>
#include <vector>

void readGmsh(const std::string filename, blaze::DynamicMatrix<double>* gcoord, blaze::DynamicMatrix<size_t>* nodes, size_t dim, size_t &element_type)
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
    
    (*gcoord)(nodeTags[i] - 1,0) =  nodeCoords[index];
    (*gcoord)(nodeTags[i] - 1,1) =  nodeCoords[index+1];
    (*gcoord)(nodeTags[i] - 1,2) =  nodeCoords[index+2];
  }

 // Check for element types
  // 2 = 3-node triangle
  // 3 = 4-node square

  size_t type = 0;
  size_t element_id;

  auto f2 = std::find(elemTypes.begin(), elemTypes.end(), 2);
  auto f3 = std::find(elemTypes.begin(), elemTypes.end(), 3);

  if (f2 != elemTypes.end()) {
    type = 2;
    element_id = f2 - elemTypes.begin();
    element_type = 5;
  } else if (f3 != elemTypes.end()) {
    type = 3;
    element_id = f3 - elemTypes.begin();
    element_type = 9;
  }

  if (f2 != elemTypes.end() and f3 != elemTypes.end())
    std::cerr << "Error: Only mesh with one type of elements is supported!"
              << std::endl;

  if (type == 0) {
    std::cerr << "Error: Only 3-node triangle or 4-node square elements are "
                 "supported!"
              << std::endl;
    exit(1);
  }

  size_t con_size = 0;

  if (type == 2) con_size = 3;

  if (type == 3) con_size = 4;

  nodes->resize(elemNodeTags[element_id].size() / con_size , con_size);

  size_t index = 0;
  for (size_t j = 0; j < elemNodeTags[element_id].size() / con_size; j++) {
    if (con_size == 3) {
      index = j * 3;

      (*nodes)(j,0) = elemNodeTags[element_id][index] - 1;
      (*nodes)(j,1) = elemNodeTags[element_id][index+1] - 1;
      (*nodes)(j,2) = elemNodeTags[element_id][index+2] - 1;

    }

    if (con_size == 4) {
      index = j * 4;

      (*nodes)(j,0) = elemNodeTags[element_id][index] - 1;
      (*nodes)(j,1) = elemNodeTags[element_id][index+1] - 1;
      (*nodes)(j,2) = elemNodeTags[element_id][index+2] - 1;
      (*nodes)(j,3) = elemNodeTags[element_id][index+3] - 1;
    }
  }


  gmsh::clear();
  gmsh::finalize();


}







int main(void){

    size_t element_type;
    blaze::DynamicMatrix<double> gcoord;
    blaze::DynamicMatrix<size_t> nodes;

    readGmsh("../../untitled.msh", &gcoord, &nodes, 2,element_type);

        
    return EXIT_SUCCESS;

}
