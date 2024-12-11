//
//  basic_camera.h
//  test
//
//  Created by Nazirul Hasan on 10/9/23.
//  modified by Badiuzzaman on 3/11/24.
//

#ifndef basic_camera_h
#define basic_camera_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    YAW_R,
    YAW_L,
    PITCH_L,
    PITCH_R,
    ROLL_L,
    ROLL_R,
};

class BasicCamera {
public:

    glm::vec3 eye;
    glm::vec3 lookAt,direction;
    glm::vec3 V;
    float Yaw, Pitch, Roll;
    float MouseSensitivity;
    float Zoom;
    glm::mat4 viewRot = glm::mat4(1.0f); // for pitch, yaw, and roll implementation

    BasicCamera(float eyeX = 0.0, float eyeY = 1.0, float eyeZ = 3.0, float lookAtX = 0.0, float lookAtY = 0.0, float lookAtZ = 0.0, glm::vec3 viewUpVector = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        eye = glm::vec3(eyeX, eyeY, eyeZ);
        lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
        V = viewUpVector;

        Yaw = 0.0f;
        Pitch = 0.0f;
        Roll = 0.0f;
        Zoom = 45.0;
        direction = glm::normalize(eye - lookAt);

    }

    glm::mat4 createViewMatrix()
    {
        direction = glm::normalize(eye - lookAt);
        return glm::lookAt(eye,lookAt, V);
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void move(Camera_Movement direction, float deltaTime)
    {
        float velocity = 2.5 * deltaTime;
        
        if (direction == FORWARD) {
            eye[2] -= velocity;
            lookAt[2] -= velocity;
        }
        if (direction == BACKWARD) {
            eye[2] += velocity;
            lookAt[2] += velocity;
        }
        if (direction == LEFT) {
            eye[0] -= velocity;
            lookAt[0] -= velocity;
        }
        if (direction == RIGHT) {
            eye[0] += velocity;
            lookAt[0] += velocity;
        }
        if (direction == YAW_R) {
            Yaw -= .15 * velocity;
        }
        if (direction == YAW_L) {
            Yaw += .15 * velocity;
        }
        if (direction == PITCH_R) {
            Pitch -= .15 * velocity;
        }
        if (direction == PITCH_L) {
            Pitch += .15 * velocity;
        }
        if (direction == ROLL_R) {
            Roll -= .15 * velocity;
        }
        if (direction == ROLL_L) {
            Roll += .15 * velocity;
        }
        updateCameraVectors();
    }

    glm::vec3 getDirection()
    {
        direction = lookAt - eye;
        return direction;
    }
    void changeLookAt(float lookAtX, float lookAtY, float lookAtZ)
    {
        lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
    }
    
    void changeViewUpVector(glm::vec3 viewUpVector)
    {
        V = viewUpVector;
    }


    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        Yaw += xoffset * MouseSensitivity;
        Pitch += yoffset * MouseSensitivity;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

private:
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 n;

    // calculates the front vector from the Camera's (updated) Euler Angles
    //void updateCameraVectors()
    //{
    //    // calculate the new Front vector
    //    glm::vec3 front;
    //    front.z = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    //    front.y = sin(glm::radians(Pitch));
    //    front.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    //    V = glm::normalize(front);
    //}

    void updateCameraVectors()
    {
        // Create the yaw rotation matrix (rotation around the Y-axis)
        glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));

        // Create the pitch rotation matrix (rotation around the X-axis)
        glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        // Create the roll rotation matrix (rotation around the Z-axis)
        glm::mat4 rollRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), glm::vec3(0.0f, 0.0f, 1.0f));

        // Combine rotations: yaw -> pitch -> roll
        glm::mat4 rotation = rollRotation * pitchRotation * yawRotation;

        // Apply rotation to the direction vector (forward direction)
        glm::vec4 transformedDirection = rotation * glm::vec4(direction, 0.0f);
        direction = glm::normalize(glm::vec3(transformedDirection));

        // Apply rotation to the up vector
        glm::vec4 transformedUp = rotation * glm::vec4(V, 0.0f);
        V = glm::normalize(glm::vec3(transformedUp));

        // Update the lookAt vector based on the direction
    }


    //void updateCameraVectors()
    //{
    //    // Create the yaw rotation matrix (rotation around the Y-axis)
    //    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));

    //    // Create the pitch rotation matrix (rotation around the X-axis)
    //    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    //    // Create the roll rotation matrix (rotation around the Z-axis)
    //    glm::mat4 rollRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), glm::vec3(0.0f, 0.0f, 1.0f));

    //    // Combine rotations: yaw -> pitch -> roll
    //    glm::mat4 rotation = rollRotation * pitchRotation * yawRotation;

    //    // Apply rotation to the camera's front direction
    //    glm::vec4 transformedDirection = rotation * glm::vec4(direction, 0.0f);
    //    direction = glm::normalize(glm::vec3(transformedDirection));

    //    // Apply rotation to the camera's up vector
    //    glm::vec4 transformedUp = rotation * glm::vec4(V, 0.0f);
    //    V = glm::normalize(glm::vec3(transformedUp));
    //}

};

#endif /* basic_camera_h */
#pragma once

