#pragma once

#include "DrawDebugHelpers.h"
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);
#define DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);
#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_SPHERE_SingleFrame_Radius(Location, Radius) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, Radius, 12, FColor::Red, false, -1.f);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT_SingleFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
	{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1.0f); \
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
	}
#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if (GetWorld()) \
	{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1.0f); \
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, false, -1.0f); \
	}
#define DRAW_ARROW_VECTOR(Location, EndLocation) if (GetWorld()) \
	{ \
	DrawDebugLine(GetWorld(), Location, EndLocation, FColor::Orange, true, -1.0f, 0, 1.0f); \
	DrawDebugAltCone(GetWorld(), EndLocation, RotateArrow, 20.0f, 0.25f, 0.50f, FColor::Red, true, -1.0f, 0U, 2.00f); \
	}
//Single frame version
#define DRAW_ARROW_VECTOR_SingleFrame(Location, EndLocation) if (GetWorld()) \
	{ \
	DrawDebugLine(GetWorld(), Location, EndLocation, FColor::Orange, false, -1.f, 0, 1.f); \
	DrawDebugAltCone(GetWorld(), EndLocation, RotateArrow, 20.0f, 0.25f, 0.50f, FColor::Red, false, -1.0f, 0U, 2.00f); \
	}
