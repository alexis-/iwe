/*******************************************************************************
Copyright (c) 2005-2009 David Williams

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*******************************************************************************/

namespace PolyVox
{
	template< template<typename> class VolumeType, typename VoxelType>
	const uint32_t CubicSurfaceExtractor<VolumeType, VoxelType>::MaxQuadsSharingVertex = 4;

	template< template<typename> class VolumeType, typename VoxelType>
	CubicSurfaceExtractor<VolumeType, VoxelType>::CubicSurfaceExtractor(VolumeType<VoxelType>* volData, Region region, SurfaceMesh<PositionMaterial>* result, bool bMergeQuads)
		:m_volData(volData)
		,m_regSizeInVoxels(region)
		,m_meshCurrent(result)
		,m_bMergeQuads(bMergeQuads)
	{
		m_meshCurrent->clear();
	}

	template< template<typename> class VolumeType, typename VoxelType>
	void CubicSurfaceExtractor<VolumeType, VoxelType>::execute()
	{
		uint32_t uArrayWidth = m_regSizeInVoxels.getUpperCorner().getX() - m_regSizeInVoxels.getLowerCorner().getX() + 2;
		uint32_t uArrayHeight = m_regSizeInVoxels.getUpperCorner().getY() - m_regSizeInVoxels.getLowerCorner().getY() + 2;

		uint32_t arraySize[3]= {uArrayWidth, uArrayHeight, MaxQuadsSharingVertex};
		m_previousSliceVertices.resize(arraySize);
		m_currentSliceVertices.resize(arraySize);
		memset(m_previousSliceVertices.getRawData(), 0xff, m_previousSliceVertices.getNoOfElements() * sizeof(IndexAndMaterial));
		memset(m_currentSliceVertices.getRawData(), 0xff, m_currentSliceVertices.getNoOfElements() * sizeof(IndexAndMaterial));

		uint32_t uRegionWidth  = m_regSizeInVoxels.getUpperCorner().getX() - m_regSizeInVoxels.getLowerCorner().getX() + 1;
		uint32_t uRegionHeight = m_regSizeInVoxels.getUpperCorner().getY() - m_regSizeInVoxels.getLowerCorner().getY() + 1;
		uint32_t uRegionDepth  = m_regSizeInVoxels.getUpperCorner().getZ() - m_regSizeInVoxels.getLowerCorner().getZ() + 1;

		m_vecQuads[NegativeX].resize(m_regSizeInVoxels.getUpperCorner().getX() - m_regSizeInVoxels.getLowerCorner().getX() + 2);
		m_vecQuads[PositiveX].resize(m_regSizeInVoxels.getUpperCorner().getX() - m_regSizeInVoxels.getLowerCorner().getX() + 2);

		m_vecQuads[NegativeY].resize(m_regSizeInVoxels.getUpperCorner().getY() - m_regSizeInVoxels.getLowerCorner().getY() + 2);
		m_vecQuads[PositiveY].resize(m_regSizeInVoxels.getUpperCorner().getY() - m_regSizeInVoxels.getLowerCorner().getY() + 2);

		m_vecQuads[NegativeZ].resize(m_regSizeInVoxels.getUpperCorner().getZ() - m_regSizeInVoxels.getLowerCorner().getZ() + 2);
		m_vecQuads[PositiveZ].resize(m_regSizeInVoxels.getUpperCorner().getZ() - m_regSizeInVoxels.getLowerCorner().getZ() + 2);

		typename VolumeType<VoxelType>::Sampler volumeSampler(m_volData);	
		Quad quad;
		
		for(int32_t z = m_regSizeInVoxels.getLowerCorner().getZ(); z <= m_regSizeInVoxels.getUpperCorner().getZ() + 1; z++)
		{
			uint32_t regZ = z - m_regSizeInVoxels.getLowerCorner().getZ();
			bool finalZ = (z == m_regSizeInVoxels.getUpperCorner().getZ() + 1);

			for(int32_t y = m_regSizeInVoxels.getLowerCorner().getY(); y <= m_regSizeInVoxels.getUpperCorner().getY() + 1; y++)
			{
				uint32_t regY = y - m_regSizeInVoxels.getLowerCorner().getY();
				bool finalY = (y == m_regSizeInVoxels.getUpperCorner().getY() + 1);

				for(int32_t x = m_regSizeInVoxels.getLowerCorner().getX(); x <= m_regSizeInVoxels.getUpperCorner().getX() + 1; x++)
				{
					uint32_t regX = x - m_regSizeInVoxels.getLowerCorner().getX();
					bool finalX = (x == m_regSizeInVoxels.getUpperCorner().getX() + 1);					

					volumeSampler.setPosition(x,y,z);

					VoxelType currentVoxel = volumeSampler.getVoxel();
					bool currentVoxelIsSolid = currentVoxel.getDensity() >= VoxelType::getThreshold();

					VoxelType negXVoxel = volumeSampler.peekVoxel1nx0py0pz();
					bool negXVoxelIsSolid = negXVoxel.getDensity()  >= VoxelType::getThreshold();

					if((currentVoxelIsSolid != negXVoxelIsSolid) && (finalY == false) && (finalZ == false))
					{
						uint32_t material = (std::max)(currentVoxel.getMaterial(), negXVoxel.getMaterial());

						// Check to ensure that when a voxel solid/non-solid change is right on a region border, the vertices are generated on the solid side of the region border
						if(((currentVoxelIsSolid > negXVoxelIsSolid) && finalX == false) || ((currentVoxelIsSolid < negXVoxelIsSolid) && regX != 0))
						{
							uint32_t v0 = addVertex(regX - 0.5f, regY - 0.5f, regZ - 0.5f, material, m_previousSliceVertices);
							uint32_t v1 = addVertex(regX - 0.5f, regY - 0.5f, regZ + 0.5f, material, m_currentSliceVertices);	
							uint32_t v2 = addVertex(regX - 0.5f, regY + 0.5f, regZ + 0.5f, material, m_currentSliceVertices);							
							uint32_t v3 = addVertex(regX - 0.5f, regY + 0.5f, regZ - 0.5f, material, m_previousSliceVertices);

							if(currentVoxelIsSolid > negXVoxelIsSolid)
							{								
								quad.vertices[0] = v0;
								quad.vertices[1] = v1;
								quad.vertices[2] = v2;
								quad.vertices[3] = v3;

								m_vecQuads[NegativeX][regX].push_back(quad);
							}
							else											
							{
								quad.vertices[0] = v0;
								quad.vertices[1] = v3;
								quad.vertices[2] = v2;
								quad.vertices[3] = v1;

								m_vecQuads[PositiveX][regX].push_back(quad);
							}

						}
					}

					VoxelType negYVoxel = volumeSampler.peekVoxel0px1ny0pz();
					bool negYVoxelIsSolid = negYVoxel.getDensity()  >= VoxelType::getThreshold();

					if((currentVoxelIsSolid != negYVoxelIsSolid) && (finalX == false) && (finalZ == false))
					{
						int material = (std::max)(currentVoxel.getMaterial(),negYVoxel.getMaterial());

						if(((currentVoxelIsSolid > negYVoxelIsSolid) && finalY == false) || ((currentVoxelIsSolid < negYVoxelIsSolid) && regY != 0))
						{
							uint32_t v0 = addVertex(regX - 0.5f, regY - 0.5f, regZ - 0.5f, material, m_previousSliceVertices);
							uint32_t v1 = addVertex(regX - 0.5f, regY - 0.5f, regZ + 0.5f, material, m_currentSliceVertices);							
							uint32_t v2 = addVertex(regX + 0.5f, regY - 0.5f, regZ + 0.5f, material, m_currentSliceVertices);
							uint32_t v3 = addVertex(regX + 0.5f, regY - 0.5f, regZ - 0.5f, material, m_previousSliceVertices);

							if(currentVoxelIsSolid > negYVoxelIsSolid)
							{
								//NOTE: For some reason y windong is opposite of X and Z. Investigate this...
								quad.vertices[0] = v0;
								quad.vertices[1] = v3;
								quad.vertices[2] = v2;
								quad.vertices[3] = v1;

								m_vecQuads[NegativeY][regY].push_back(quad);
							}
							else
							{
								//NOTE: For some reason y windong is opposite of X and Z. Investigate this...
								quad.vertices[0] = v0;
								quad.vertices[1] = v1;
								quad.vertices[2] = v2;
								quad.vertices[3] = v3;

								m_vecQuads[PositiveY][regY].push_back(quad);
							}
						}
					}

					VoxelType negZVoxel = volumeSampler.peekVoxel0px0py1nz();
					bool negZVoxelIsSolid = negZVoxel.getDensity()  >= VoxelType::getThreshold();

					if((currentVoxelIsSolid != negZVoxelIsSolid) && (finalX == false) && (finalY == false))
					{
						int material = (std::max)(currentVoxel.getMaterial(), negZVoxel.getMaterial());

						if(((currentVoxelIsSolid > negZVoxelIsSolid) && finalZ == false) || ((currentVoxelIsSolid < negZVoxelIsSolid) && regZ != 0))
						{
							uint32_t v0 = addVertex(regX - 0.5f, regY - 0.5f, regZ - 0.5f, material, m_previousSliceVertices);
							uint32_t v1 = addVertex(regX - 0.5f, regY + 0.5f, regZ - 0.5f, material, m_previousSliceVertices);
							uint32_t v2 = addVertex(regX + 0.5f, regY + 0.5f, regZ - 0.5f, material, m_previousSliceVertices);
							uint32_t v3 = addVertex(regX + 0.5f, regY - 0.5f, regZ - 0.5f, material, m_previousSliceVertices);							
	
							if(currentVoxelIsSolid > negZVoxelIsSolid)
							{
								quad.vertices[0] = v0;
								quad.vertices[1] = v1;
								quad.vertices[2] = v2;
								quad.vertices[3] = v3;

								m_vecQuads[NegativeZ][regZ].push_back(quad);
							}
							else
							{
								quad.vertices[0] = v0;
								quad.vertices[1] = v3;
								quad.vertices[2] = v2;
								quad.vertices[3] = v1;

								m_vecQuads[PositiveZ][regZ].push_back(quad);
							}
						}
					}
				}
			}

			m_previousSliceVertices.swap(m_currentSliceVertices);
			memset(m_currentSliceVertices.getRawData(), 0xff, m_currentSliceVertices.getNoOfElements() * sizeof(IndexAndMaterial));
		}

		for(uint32_t uFace = 0; uFace < NoOfFaces; uFace++)
		{
			std::vector< std::list<Quad> >& vecListQuads = m_vecQuads[uFace];

			for(uint32_t slice = 0; slice < vecListQuads.size(); slice++)
			{
				std::list<Quad>& listQuads = vecListQuads[slice];

				if(m_bMergeQuads)
				{
					//Repeatedly call this function until it returns
					//false to indicate nothing more can be done.
					while(performQuadMerging(listQuads)){}
				}

				typename std::list<Quad>::iterator iterEnd = listQuads.end();
				for(typename std::list<Quad>::iterator quadIter = listQuads.begin(); quadIter != iterEnd; quadIter++)
				{
					Quad& quad = *quadIter;				
					m_meshCurrent->addTriangleCubic(quad.vertices[0], quad.vertices[1],quad.vertices[2]);
					m_meshCurrent->addTriangleCubic(quad.vertices[0], quad.vertices[2],quad.vertices[3]);
				}			
			}
		}

		m_meshCurrent->m_Region = m_regSizeInVoxels;
		m_meshCurrent->removeUnusedVertices();

		m_meshCurrent->m_vecLodRecords.clear();
		LodRecord lodRecord;
		lodRecord.beginIndex = 0;
		lodRecord.endIndex = m_meshCurrent->getNoOfIndices();
		m_meshCurrent->m_vecLodRecords.push_back(lodRecord);
	}

	template< template<typename> class VolumeType, typename VoxelType>
	int32_t CubicSurfaceExtractor<VolumeType, VoxelType>::addVertex(float fX, float fY, float fZ, uint32_t uMaterialIn, Array<3, IndexAndMaterial>& existingVertices)
	{
		uint32_t uX = static_cast<uint32_t>(fX + 0.75f);
		uint32_t uY = static_cast<uint32_t>(fY + 0.75f);

		for(uint32_t ct = 0; ct < MaxQuadsSharingVertex; ct++)
		{
			IndexAndMaterial& rEntry = existingVertices[uX][uY][ct];

			if(rEntry.iIndex == -1)
			{
				//No vertices matched and we've now hit an empty space. Fill it by creating a vertex.
				rEntry.iIndex = m_meshCurrent->addVertex(PositionMaterial(Vector3DFloat(fX, fY, fZ), uMaterialIn));
				rEntry.uMaterial = uMaterialIn;

				return rEntry.iIndex;
			}

			//If we have an existing vertex and the material matches then we can return it.
			if(rEntry.uMaterial == uMaterialIn)
			{
				return rEntry.iIndex;
			}
		}

		//If we exit the loop here then apparently all the slots were full but none of
		//them matched. I don't think this can happen so let's put an assert to make sure.
		assert(false);
		return 0;
	}

	template< template<typename> class VolumeType, typename VoxelType>
	bool CubicSurfaceExtractor<VolumeType, VoxelType>::performQuadMerging(std::list<Quad>& quads)
	{
		bool bDidMerge = false;
		for(typename std::list<Quad>::iterator outerIter = quads.begin(); outerIter != quads.end(); outerIter++)
		{
			typename std::list<Quad>::iterator innerIter = outerIter;
			innerIter++;
			while(innerIter != quads.end())
			{
				Quad& q1 = *outerIter;
				Quad& q2 = *innerIter;

				bool result = mergeQuads(q1,q2);

				if(result)
				{
					bDidMerge = true;
					innerIter = quads.erase(innerIter);
				}
				else
				{
					innerIter++;
				}
			}
		}

		return bDidMerge;
	}

	template< template<typename> class VolumeType, typename VoxelType>
	bool CubicSurfaceExtractor<VolumeType, VoxelType>::mergeQuads(Quad& q1, Quad& q2)
	{
		//All four vertices of a given quad have the same material,
		//so just check that the first pair or vertices match.
		if(fabs(m_meshCurrent->getVertices()[q1.vertices[0]].getMaterial() - m_meshCurrent->getVertices()[q2.vertices[0]].getMaterial()) < 0.001)
		{
			//Now check whether quad 2 is adjacent to quad one by comparing vertices.
			//Adjacent quads must share two vertices, and the second quad could be to the
			//top, bottom, left, of right of the first one. This gives four combinations to test.
			if((q1.vertices[0] == q2.vertices[1]) && ((q1.vertices[3] == q2.vertices[2])))
			{
				q1.vertices[0] = q2.vertices[0];
				q1.vertices[3] = q2.vertices[3];
				return true;
			}
			else if((q1.vertices[3] == q2.vertices[0]) && ((q1.vertices[2] == q2.vertices[1])))
			{
				q1.vertices[3] = q2.vertices[3];
				q1.vertices[2] = q2.vertices[2];
				return true;
			}
			else if((q1.vertices[1] == q2.vertices[0]) && ((q1.vertices[2] == q2.vertices[3])))
			{
				q1.vertices[1] = q2.vertices[1];
				q1.vertices[2] = q2.vertices[2];
				return true;
			}
			else if((q1.vertices[0] == q2.vertices[3]) && ((q1.vertices[1] == q2.vertices[2])))
			{
				q1.vertices[0] = q2.vertices[0];
				q1.vertices[1] = q2.vertices[1];
				return true;
			}
		}
		
		//Quads cannot be merged.
		return false;
	}
}