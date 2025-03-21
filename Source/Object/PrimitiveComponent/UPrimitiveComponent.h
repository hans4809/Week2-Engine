#pragma once

#include "Core/Engine.h"
#include "Object/USceneComponent.h"
#include "Primitive/PrimitiveVertices.h"
#include "Resource/RenderResourceCollection.h"


//상수버퍼로 객체의 정보를 넣을 구조체
struct alignas(16) FConstantsComponentData
{
	FMatrix MVP;
	FVector4 Color;
	// true인 경우 Vertex Color를 사용하고, false인 경우 Color를 사용합니다.
	FVector4 UUIDColor;
	uint32 bUseVertexColor;
	FVector Padding;
};

class UPrimitiveComponent : public USceneComponent
{
	DECLARE_CLASS(UPrimitiveComponent, USceneComponent)

public:
	UPrimitiveComponent();
	virtual ~UPrimitiveComponent() override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//void UpdateConstantPicking(const URenderer& Renderer, FVector4 UUIDColor) const;
	//void UpdateConstantDepth(const URenderer& Renderer, int Depth) const;
	virtual void Render();
	virtual void CalculateModelMatrix(FMatrix& OutMatrix);

	virtual EPrimitiveType GetType() { return EPrimitiveType::EPT_None; }

	bool IsUseVertexColor() const { return bUseVertexColor; }

	void SetCustomColor(const FVector4& InColor)
	{
		CustomColor = InColor; 
		bUseVertexColor = false;
	}

	void SetUseVertexColor(bool bUse)
	{
		bUseVertexColor = bUse;
	}
	const FVector4& GetCustomColor() const { return CustomColor; }

public:
	virtual void RegisterComponentWithWorld(class UWorld* World);

public:
	void SetCanBeRendered(bool bRender) { bCanBeRendered = bRender; }

	void SetIsOrthoGraphic(bool IsOrtho) { bIsOrthoGraphic = IsOrtho; }
	bool GetIsOrthoGraphic() { return bIsOrthoGraphic;}

	void SetIsPicking(bool IsPicking) { bIsPicking = IsPicking; }
	bool GetIsPicking() { return bIsPicking; }
	FConstantsComponentData& GetConstantsComponentData() { return ConstantsComponentData; }
	//void SetConstantsComponentData(FConstantsComponentData& ) { bIsBillboard = bBillboard; }

	std::shared_ptr<class FMesh> GetMesh() { return RenderResourceCollection.GetMesh(); }
	std::shared_ptr<class FMaterial> GetMaterial() { return RenderResourceCollection.GetMaterial(); }

	void SetMesh(const FString& _Name) { RenderResourceCollection.SetMesh(_Name); }
	void SetMaterial(const FString& _Name) { RenderResourceCollection.SetMaterial(_Name); }
	
	FRenderResourceCollection& GetRenderResourceCollection() { return RenderResourceCollection; }
public:
	void SetBoundsScale(float NewBoudnsScale);

	virtual void UpdateBounds() override;
protected:
	float BoundsScale = 1.0f;
protected:
	bool bCanBeRendered = false;
	bool bIsBillboard = false;
	bool bUseVertexColor = true;
	bool bIsOrthoGraphic = false;
	bool bIsPicking = false;

	FVector4 CustomColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
protected:
	FRenderResourceCollection RenderResourceCollection;
	FConstantsComponentData ConstantsComponentData;

public:
	void SetBoxExtent(const FVector& InExtent);

	inline FVector GetScaledBoxExtent() const { return BoxExtent * GetComponentTransform().GetScale(); }

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	inline void InitBoxExtent(const FVector& InExtent) { BoxExtent = InExtent; }

public:
	FVector BoxExtent;
};

class UCubeComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UCubeComp, UPrimitiveComponent)

public:
	UCubeComp();

	EPrimitiveType GetType() override
	{

		return EPrimitiveType::EPT_Cube;
	}
};

class USphereComp : public UPrimitiveComponent
{
	DECLARE_CLASS(USphereComp, UPrimitiveComponent)

public:
	USphereComp();

	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Sphere;
	}

	/**
	* Change the sphere radius. This is the unscaled radius, before component scale is applied.
	* @param	InSphereRadius: the new sphere radius
	* @param	bUpdateOverlaps: if true and this shape is registered and collides, updates touching array for owner actor.
	*/
	void SetSphereRadius(float InSphereRadius);

	// @return the radius of the sphere, with component scale applied.
	inline float GetScaledSphereRadius() const { return Radius * GetShapeScale(); }

	// @return the radius of the sphere, ignoring component scale.
	inline float GetUnscaledSphereRadius() const { return Radius; }

	//~ Begin UPrimitiveComponent Interface.
	virtual inline bool IsZeroExtent() { return Radius == 0.0f; };
	//virtual struct FCollisionShape GetCollisionShape(float Inflation = 0.0f) const override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin USceneComponent Interface
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ End USceneComponent Interface

	// Get the scale used by this shape. This is a uniform scale that is the minimum of any non-uniform scaling.
	// @return the scale used by this shape.
	float GetShapeScale() const;

	// Sets the sphere radius without triggering a render or physics update.
	inline void InitSphereRadius(float InSphereRadius) { Radius = InSphereRadius; }
protected:
	float Radius;
};

class UTriangleComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UTriangleComp, UPrimitiveComponent)

public:
	UTriangleComp();

	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Triangle;
	}
};

class ULineComp : public UPrimitiveComponent
{
	DECLARE_CLASS(ULineComp, UPrimitiveComponent)


public:
	ULineComp();

	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Line;
	}
};

class UCylinderComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UCylinderComp, UPrimitiveComponent)


public:
	UCylinderComp();

	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cylinder;
	}
};

class UConeComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UConeComp, UPrimitiveComponent)

public:
	UConeComp();

	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cone;
	}
};

class UQuadComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UQuadComp, UPrimitiveComponent)

public:
	UQuadComp();

	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Quad;
	}
};