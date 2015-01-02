glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

glm::mat4 projection_matrix()
{
	float zNear = 0.001f;
	float zFar = 1000.0f;
	float fovy = 45; // glm::pi<float>() * 0.25f; // 45°
	float aspect = WINDOW_ASPECT_RATIO;

	float left = -aspect;
	float right = aspect;
	float bottom = -1.0f;
	float top = 1.0f;

	glm::mat4 Projection = glm::perspective(fovy, aspect, zNear, zFar);
	//glm::mat4 Projection = glm::ortho(left, right, bottom, top, zNear, zFar);

	return Projection;
}

glm::mat4 model_matrix(glm::vec3& position, glm::vec3& rotation, glm::vec3& scaling)
{
	glm::mat4 IdealMatrix(1);

	glm::mat4 Scaling = glm::scale(IdealMatrix, scaling);

	glm::mat4 Translation = glm::translate(IdealMatrix, position);

	glm::mat4 Rotation = IdealMatrix;
	Rotation *= glm::rotate(IdealMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	Rotation *= glm::rotate(IdealMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	Rotation *= glm::rotate(IdealMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 Model = Translation * Rotation * Scaling;

	return Model;
}

glm::mat4 view_matrix(glm::vec3& position, glm::vec3& direction, glm::vec3& up)
{
	glm::vec3 eye = position;
	glm::vec3 center = position + direction;

	glm::mat4 View = glm::lookAt(eye, center, up);

	return View;
}

glm::mat4 mvp_matrix(double deltaTime)
{
	return Projection * View * Model;
}