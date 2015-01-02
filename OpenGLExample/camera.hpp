float pi = glm::pi<float>(); // RAD
float deg90 = pi * 0.5f; // degree 90° in RAD
float deg360 = pi * 2.0f; // degree 360° in RAD

float rad_width = deg360 / (WINDOW_WIDTH * 1.0f); // 0.001f 
float rad_height = deg360 / (WINDOW_HEIGHT * 1.0f); // 0.001f 

glm::vec3 camCurrentRotation;

glm::vec3 camPosition;
glm::vec3 camRotation;
glm::vec3 camDirection;
glm::vec3 camUp(0, 1, 0);
glm::vec3 camRight(1, 0, 0);

void camera_rotation(double deltaTime, float mouseDifX, float mouseDifY)
{
	if (!rightMouseHold)
	{
		float time = (float)deltaTime * 2;

		if (ROTATE_LEFT)
		{
			camRotation.x += time;
		}
		if (ROTATE_RIGHT)
		{
			camRotation.x -= time;
		}
		if (ROTATE_UP)
		{
			camRotation.y += time;
		}
		if (ROTATE_DOWN)
		{
			camRotation.y -= time;
		}
	}

	if (camRotation.x >= deg360) camRotation.x = deg360 - camRotation.x;
	if (camRotation.x <= -deg360) camRotation.x = deg360 + camRotation.x;

	if (camRotation.y >= deg360) camRotation.y = deg360 - camRotation.y;
	if (camRotation.y <= -deg360) camRotation.y = deg360 + camRotation.y;

	glm::vec3 rotation = camRotation;

	if (rightMouseHold)
	{
		// Compute new orientation
		rotation.x -= mouseDifX * rad_width; // horizontalAngle
		rotation.y -= mouseDifY * rad_height; // verticalAngle
	}
	
	camCurrentRotation = rotation;

	// --------------------------------------------------------------------

	// Direction
	camDirection = glm::vec3(
		cos(rotation.y) * sin(rotation.x),	// cos(verticalAngle) * sin(horizontalAngle)
		sin(rotation.y),							// sin(verticalAngle)
		cos(rotation.y) * cos(rotation.x)		// cos(verticalAngle) * cos(horizontalAngle)
		);

	// right vector
	camRight = glm::vec3(
		sin(rotation.x - deg90),
		0.0f,
		cos(rotation.x - deg90)
		);

	camUp = glm::cross(camRight, camDirection); // up vector
}

void camera_postion(double deltaTime)
{
	glm::vec3 time((float)deltaTime * 2);

	if (FORWARD)
	{
		camPosition += camDirection * time;
	}
	if (BACK)
	{
		camPosition -= camDirection * time;
	}
	if (LEFT)
	{
		camPosition -= camRight * time;
	}
	if (RIGHT)
	{
		camPosition += camRight * time;
	}
	if (UP)
	{
		camPosition += camUp * time;
	}
	if (DOWN)
	{
		camPosition -= camUp * time;
	}
}

void camera_movement(double deltaTime, float mouseDifX, float mouseDifY)
{
	camera_rotation(deltaTime, mouseDifX, mouseDifY);
	camera_postion(deltaTime);
}