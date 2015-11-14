#include "Sonic.h"

Sonic::Sonic(vector<string> frames)
{
	vector<GLuint> elements = vector<GLuint>();

	this->numFrames = frames.size();

	/* Loading meshes */

	meshes = new XBO*[this->numFrames];


	this->vaos = new VertexArrayObject**[this->numFrames];

	for(int i = 0; i < this->numFrames; i++) 
		meshes[i] = FromFile(frames[i]);

	/* Loading run animation */

	for(int i = 0; i < this->numFrames; i++) {
		XBO * m1 = meshes[i % this->numFrames];
		XBO * m2 = meshes[(i + 1) % this->numFrames];
		
		#ifdef GL_UTILS_LOG_ENABLED
			char buf[5];
			sprintf(buf, "%d", i);
			Logger::getDefaultLogger()->writeMessage(0, "Sonic::Sonic()", "Loading animation key-frame: " + string(buf));
		#endif

		this->vaos[i] = new VertexArrayObject*[m1->ShadingGroupCount];

		for(int j = 0; j < m1->ShadingGroupCount; j++) {

			this->vaos[i][j] = new VertexArrayObject(2, GL_STATIC_DRAW);

			this->vaos[i][j]->setBufferData(0, sizeof(ExtendedBinary3DObject::Vertex) * m1->ShadingGroups[j].VerticesCount, m1->ShadingGroups[j].Vertices);
			this->vaos[i][j]->setBufferData(1, sizeof(ExtendedBinary3DObject::Vertex) * m2->ShadingGroups[j].VerticesCount, m2->ShadingGroups[j].Vertices);
			
			this->vaos[i][j]->setVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), 0);
			this->vaos[i][j]->setVertexAttribute(0, 2, 3, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), (char*)0 + 12);
			this->vaos[i][j]->setVertexAttribute(1, 1, 3, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), 0);
			this->vaos[i][j]->setVertexAttribute(1, 3, 3, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), (char*)0 + 12);

			this->vaos[i][j]->enableVertexAttribute(0);
			this->vaos[i][j]->enableVertexAttribute(1);
			this->vaos[i][j]->enableVertexAttribute(2);
			this->vaos[i][j]->enableVertexAttribute(3);
		}
	}

}

Sonic::~Sonic(void)
{
	delete[] this->meshes;
}

int Sonic::GetFramesCount() { return this->numFrames; }

void Sonic::DrawAnimation(Shader * shader, GLuint frame) {

	for(int i = 0; i < this->meshes[frame]->ShadingGroupCount; i++) {
		
		shader->setUniform("in_Color", this->meshes[frame]->ShadingGroups[i].Material.Diffuse);

		glBindVertexArray(this->vaos[frame][i]->getID());
		glDrawArrays(GL_TRIANGLES, 0, this->meshes[frame]->ShadingGroups[i].VerticesCount);
		glBindVertexArray(0);
	}
}