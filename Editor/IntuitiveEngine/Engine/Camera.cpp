#include "stdafx.h"

#include <OgreCamera.h>
#include "Camera.h"

CCamera::CCamera(Ogre::Camera* pCamera)
	: m_pCamera(pCamera)
{
	if (!m_pCamera)
		throw NEW std::exception("FATAL ERROR: CCamera was instantiated with a NULL camera");
}

void
CCamera::setAspectRatio(int cx, int cy)
{
	m_pCamera->setAspectRatio(Ogre::Real(cx) / Ogre::Real(cy));
}

void
CCamera::move(const Ogre::Vector3& vDirection, float fDistance)
{
	m_pCamera->move(vDirection * fDistance);
}

void
CCamera::zoom(float delta)
{
	Ogre::Vector3 vCamPos = m_pCamera->getPosition();
	Ogre::Vector3 vCamDir = m_pCamera->getDirection();

	m_pCamera->setPosition(vCamPos + vCamDir * delta);
}

void
CCamera::rotate(float x, float y)
{
	m_pCamera->yaw(Ogre::Radian(x / 300.0f));
	m_pCamera->pitch(Ogre::Radian(y / 300.0f));
}

const Ogre::Vector3&
CCamera::getPosition()
{
	return m_pCamera->getPosition();
}

const Ogre::Vector3
CCamera::getDirection()
{
	return m_pCamera->getDirection();
}

const Ogre::Quaternion&
CCamera::getOrientation()
{
	return m_pCamera->getOrientation();
}

void
CCamera::setPosition(const Ogre::Vector3& vec)
{
	m_pCamera->setPosition(vec);
}

void
CCamera::setDirection(const Ogre::Vector3& vec)
{
	m_pCamera->setDirection(vec);
}

void
CCamera::setOrientation(const Ogre::Quaternion& quat)
{
	m_pCamera->setOrientation(quat);
}

void
CCamera::lookAt(const Ogre::Vector3& vec)
{
	m_pCamera->lookAt(vec);
}

Ogre::Camera*
CCamera::getOgreCamera()
{
  return m_pCamera;
}