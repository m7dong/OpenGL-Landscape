#include "tree.h"
#include <iostream>

Tree::Tree() {
}

Tree::Tree(int init_depth, int init_height) {
	branchTexture = Texture("textures/cortex.bmp");
	branchTexture.LoadTexture();

	depth = init_depth;
	height = init_height;

	// cortex
	Branch* cortex = new Branch(0.0f, -1.0f, 3.0f);
	branchList.push_back(cortex);

	// root branch
	Branch* root_branch = new Branch(0.0f, 3.0f);
	branchList.push_back(root_branch);


	// build the tree per level of height given depth
	Branch* level_root_branch = root_branch;
	for (int h=1; h <= height; h++) {
		buildTree(level_root_branch, init_depth, h);

		level_root_branch = new Branch(0.0f, 3.0f+h*2.0f);
		branchList.push_back(level_root_branch);
	}
}

void Tree::buildTree(Branch* root_branch, int tree_depth, int tree_height) {
	if (tree_depth == 0) {
		return;
	}

	int child_number = 2;
	for (int i=0; i < child_number; i++) {
		GLfloat angle = 80.0f * (1+tree_height) * 0.1;
		GLfloat y_translation = 1.0f*0.4;//*(tree_height+0.1);
		if (i == 1) {
			angle *= -1;
			y_translation = 3.0f*0.4;//*(tree_height+0.1);
		}
		Branch* branch = new Branch(angle, y_translation, root_branch, (tree_depth*0.5f+tree_height*0.3));
		branchList.push_back(branch);

		buildTree(branch, tree_depth-1, tree_height);

	}
}

void Tree::CreateCubeBranchMesh() {
	// initialize a cube
	int indicesLen = 36;  // 3 * 12
	int verticesLen = 64; // 8 * 8

	// 12 triangles
	unsigned int indices[] = {
		0, 1, 2,
		5, 0, 3,
		6, 0, 7,
		5, 7, 0,
		0, 2, 3, 
		6, 1, 0,
		2, 1, 6,
		4, 7, 5,
		7, 4, 6, 
		4, 5, 3,
		4, 3, 2, 
		4, 2, 6
	};

	// 8 vertices
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -1.0f,       0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //0
		-1.0f, -1.0f, 1.0f,        0.5f, 0.0f,     0.0f, -0.0f, 0.0f,  //1
		-1.0f, 1.0f, 1.0f,         1.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //2
		-1.0f, 1.0f, -1.0f,        0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //3
		1.0f, 1.0f, 1.0f,          0.0f, 1.0f,     0.0f, -0.0f, 0.0f,  //4
		1.0f, 1.0f, -1.0f,         1.0f, 1.0f,     0.0f, -0.0f, 0.0f,  //5
		1.0f, -1.0f, 1.0f,         0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //6
		1.0f, -1.0f, -1.0f,        0.0f, 0.0f,     0.0f, -0.0f, 0.0f,  //7
	};

	Mesh* cube = new Mesh();
	cube->CreateMesh(vertices, indices, verticesLen, indicesLen);
	treeMeshList.push_back(cube);
}

void Tree::CreateCylinderBranchMesh() {
	int segments = 10;
	int num_cylinder_vertices = 4 * 10;
	GLfloat cylinderVertices[40*8] = {};

	GLfloat const bottom = -1.0f;
	GLfloat const top    = 1.0f;
	int num= 0;
	GLfloat r = 0.2f;
	for(GLuint n = 0; n < segments; ++n) {
		GLfloat const t0 = 2 * M_PI * (float)n / (float)segments;
		GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segments;
		//quad vertex 0
		cylinderVertices[num++] = sin(t0) * r;  // x
		cylinderVertices[num++] = bottom;			// y
		cylinderVertices[num++] = cos(t0) * r;	// z
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		//quad vertex 1
		cylinderVertices[num++] = sin(t1) * r;
		cylinderVertices[num++] = bottom;
		cylinderVertices[num++] = cos(t1) * r;
		cylinderVertices[num++] = 0.5f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		//quad vertex 2
		cylinderVertices[num++] = sin(t1) * r;
		cylinderVertices[num++] = top;
		cylinderVertices[num++] = cos(t1) * r;
		cylinderVertices[num++] = 0.5f;
		cylinderVertices[num++] = 1.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		//quad vertex 3
		cylinderVertices[num++] = sin(t0) * r;
		cylinderVertices[num++] = top;
		cylinderVertices[num++] = cos(t0) * r;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.5f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
		cylinderVertices[num++] = 0.0f;
	}

	num = 0;
	int num_indices = 6*10;
	unsigned int cylinderIndices[6*10] = {};
	for(GLuint n = 0; n < segments; ++n) {
		//quad vertex 0
		cylinderIndices[num++] = n*4+0;  
		cylinderIndices[num++] = n*4+1;			
		cylinderIndices[num++] = n*4+2;	

		cylinderIndices[num++] = n*4+0;  
		cylinderIndices[num++] = n*4+2;		
		cylinderIndices[num++] = n*4+3;	
	}

	Mesh* cylinder = new Mesh();
	cylinder->CreateMesh(cylinderVertices, cylinderIndices, num_cylinder_vertices*8, num_indices);
	treeMeshList.push_back(cylinder);
}

void Tree::renderTree(GLuint uniformModel, GLuint uniformView, GLuint uniformProjection, glm::mat4x4 projection) {
	for (int i = 0; i < branchList.size(); i++) {
		CreateCylinderBranchMesh();
		Branch* curr_branch = branchList[i];

		curr_branch->renderBranch(uniformModel, uniformProjection, projection);
		branchTexture.UseTexture();
		treeMeshList[i]->RenderMesh();
	}
}

void Tree::keyControl(bool* keys) {
	if (keys[GLFW_KEY_G]) {
		growUpward(keys);
	}

	if (keys[GLFW_KEY_B]) {
		growDownward(keys);
	}
}

void Tree::growUpward(bool* keys) {
	
}

void Tree::growDownward(bool* keys) {
	
}

Tree::~Tree() {
}










// void SudoCreateChild(Tree& tree, const glm::mat4& m2w_noscale, const int depth)
// {
// 	int MAX_DEPTH = 3;
// 	int CHILDREN_NUMBER = 2;
// 	GLfloat PI = 3.14;

//     if(depth>=MAX_DEPTH){
//         return;
//     }
//     /// Creates the M2W matrix
//     glm::mat4 m2w=m2w_noscale * glm::mat4(1.0);*(1/depth);
//     /// Draw the branch
//     renderBranch(m2w);
//     /// Create the children branches
//     for(int i=0; i< CHILDREN_NUMBER; ++i){
//         float angle=(2.0f*PI/CHILDREN_NUMBER)*i;
//         /// Initial rotation of PI/4 plus equal rotation between siblins
// 		glm::mat4 rotation = glm::mat4(1.0);
//         rotation=glm::rotate(rotation, angle, glm::vec3(0,1,0));
// 		rotation = glm::rotate(rotation, PI*0.25, glm::vec3(1,0,0));
//         /// Size of the branch is 1/depth
// 		glm::mat4 translation = glm::mat4(1.0);
//         translation = glm::translate(translation, glm::vec3(glm::vec3(0,1,0)*(1/(depth+1))));
//         /// Recursively create the branches
//         SudoCreateChild(m2w_noscale*translation*rotation, depth+1);
//     }
// }

