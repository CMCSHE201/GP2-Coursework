#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	void Position_Camera(float pos_x, float pos_y, float pos_z,
		float view_x, float view_y, float view_z,
		float up_x, float up_y, float up_z)
	{
		mPos = glm::vec3(pos_x, pos_y, pos_z); // set position
		mView = glm::vec3(view_x, view_y, view_z); // set view
		mUp = glm::vec3(up_x, up_y, up_z); // set the up vector
	}

	void Move_Camera(float speed)
	{
		glm::vec3 vVector = mView - mPos;

		mPos.x = mPos.x + vVector.x * speed;
		mPos.z = mPos.z + vVector.z * speed;
		mView.x = mView.x + vVector.x * speed;
		mView.z = mView.z + vVector.z * speed;
	}
	/*void update(double deltaTime)
	{
		currentPos = this->getPos();

		currentPos.x += (int)(this->playerVelocity * this->xMove * deltaTime * 3);
		currentPos.y += (int)(this->playerVelocity * this->yMove * deltaTime * 3);

		this->pos.x = currentPos.x;
		this->pos.y = currentPos.y;
	}*/


	void initCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	glm::vec3 getPos()
	{
		return this->pos;
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	void MoveCameraRight(int playerMove)
	{
		xMove = playerMove;
	}

	void MoveCameraUp(int playerMove)
	{
		yMove = playerMove;
	}

	/*void MoveRight(float amt)
	{
		pos += glm::cross(up, forward) * amt;
	}*/

	//void Pitch(float angle)
	//{
	//	glm::vec3 right = glm::normalize(glm::cross(up, forward));

	//	forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
	//	up = glm::normalize(glm::cross(forward, right));
	//}

	//void RotateY(float angle)
	//{
	//	static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

	//	glm::mat4 rotation = glm::rotate(angle, UP);

	//	forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	//	up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	//}

protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 currentPos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 mPos;
	glm::vec3 mView;
	glm::vec3 mUp;
	int playerVelocity;
	int yMove;
	int xMove;
};


