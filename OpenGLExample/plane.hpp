GLuint plane_id = 0;
GLuint plane_buffer_id[2];

#define VERTEX_DATA_SIZE 12
#define INDEX_DATA_SIZE 4

glm::vec3 plane_position(0, 0, 1);
glm::vec3 plane_rotation(0, glm::pi<double>(), 0);
glm::vec3 plane_scaling(0.5f);

void plane(int mode = 0)
{
	if (mode == 0)
	{
		// RENDER PLANE
		glBindVertexArray(plane_id);

		//glDrawArrays(GL_TRIANGLES, 0, VERTEX_DATA_SIZE);
		glDrawElements(GL_TRIANGLE_STRIP, INDEX_DATA_SIZE, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		printGLError("render plane");
	}
	else if (mode == -1)
	{
		glDeleteVertexArrays(1, &plane_id);
		glDeleteBuffers(2, plane_buffer_id);
		printGLError("delete plane");
	}
	else if (mode == 1)
	{
		// CREATE PLANE

		GLuint vertex_data_Size = 0;

		GLfloat vertex_data[VERTEX_DATA_SIZE] =	{
			/*(*/ -1, -1, 0, /*)*/ 
			/*(*/ 1, -1, 0, /*)*/ 
			/*(*/ -1, 1, 0, /*)*/ 
			/*(*/ 1, 1, 0 /*)*/ 
		};

		GLuint index_data[INDEX_DATA_SIZE] = { 0, 1, 2, 3 };

		glGenVertexArrays(1, &plane_id);
		glGenBuffers(2, plane_buffer_id);

		glBindVertexArray(plane_id);

		glBindBuffer(GL_ARRAY_BUFFER, plane_buffer_id[0]);
		glBufferData(GL_ARRAY_BUFFER, VERTEX_DATA_SIZE * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_buffer_id[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_DATA_SIZE * sizeof(GLuint), index_data, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		printGLError("create plane");
	}
}