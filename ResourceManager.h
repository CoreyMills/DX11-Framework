#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <fstream>		//For loading in an external file
#include <map>			//For fast searching when re-creating the index buffer
#include "Structures.h"
#include "GameObject.h"

class ResourceManager
{
private:
	std::vector<GameObject*> _gameObjectArray;
	std::vector<ID3D11ShaderResourceView*> _textureArray;
	std::vector<Mesh*> _meshArray;

	//Helper methods for the above method
	//Searhes to see if a similar vertex already exists in the buffer -- if true, we re-use that index
	bool FindSimilarVertex(const SimpleVertex& vertex, std::map<SimpleVertex, unsigned short>& vertToIndexMap, unsigned short& index);

	//Re-creates a single index buffer from the 3 given in the OBJ file
	void CreateIndices(const std::vector<XMFLOAT3>& inVertices, const std::vector<XMFLOAT2>& inTexCoords,
		const std::vector<XMFLOAT3>& inNormals, std::vector<unsigned short>& outIndices,
		std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTexCoords, std::vector<XMFLOAT3>& outNormals);

public:
	ResourceManager();
	~ResourceManager();

	void Init();
	void Cleanup();

	/////////////////////////////////////////////////
	////3D Objects Methods/////
	void Add3DObject(GameObject* object);
	void RemoveObjectAt(int index);
	void RemoveAllObjects();

	GameObject* GetObjectByIndex(int index);
	GameObject* GetObjectByName(std::string name);
	int GetObjectArraySize() { return _gameObjectArray.size(); }

	/////////////////////////////////////////////////
	////Texture Methods/////

	//example fileName = L"Textures/Crate_COLOR.dds"
	HRESULT LoadTexture(ID3D11Device& _pd3dDevice, const wchar_t* fileName);
	void RemoveTextureAt(int index);
	void RemoveAllTextures();

	ID3D11ShaderResourceView* GetTextureAt(int index);
	int GetTextureArraySize() { return _textureArray.size(); }

	/////////////////////////////////////////////////
	////Mesh Methods/////

	void LoadOBJ(std::string meshName, char* filename, ID3D11Device* _pd3dDevice, bool invertTexCoords = true);
	void AddOBJ(Mesh* newMesh);
	void RemoveMeshAt(int index);
	void RemoveAllMeshs();

	/*do not give multiple objects the same name.
	if you do then you will recieve the first found*/
	Mesh* GetMeshByName(std::string meshName);
	Mesh* GetMeshByIndex(int index);
	int GetMeshArraySize() { return _meshArray.size(); }
};
#endif