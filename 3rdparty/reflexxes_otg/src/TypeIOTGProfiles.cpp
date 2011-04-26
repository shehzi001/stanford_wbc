//  ---------------------- Doxygen info ----------------------
//! \file TypeIOTGProfiles.cpp
//!
//! \brief
//! Implementation file for the calculation of all motion
//! profiles for the Type I On-Line Trajectory Generation algorithm
//!
//! \sa TypeIOTGProfiles.h
//! \n
//! \n
//! \b License 
//! \n
//! \n
//! This file is part of the Reflexxes Type I OTG Library.
//! \n\n
//! The Reflexxes Type I OTG Library is free software: you can redistribute
//! it and/or modify it under the terms of the GNU General Public License
//! as published by the Free Software Foundation, either version 3 of the
//! License, or (at your option) any later version.
//! \n\n
//! The Reflexxes Type I OTG Library is distributed in the hope that it
//! will be useful, but WITHOUT ANY WARRANTY; without even the implied 
//! warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
//! the GNU General Public License for more details.
//! \n\n
//! You should have received a copy of the GNU General Public License
//! along with the Reflexxes Type I OTG Library. If not, see 
//! <http://www.gnu.org/licenses/>.
//! \n
//! \n
//! \n
//! Reflexxes GmbH\n
//! Sandknoell 7\n
//! D-24805 Hamdorf\n
//! GERMANY\n
//! \n
//! http://www.reflexxes.com\n
//!
//! \date June 2010
//! 
//! \version 1.0
//!
//!	\author Torsten Kroeger, <info@reflexxes.com> \n
//!	
//!
//! \note Copyright (C) 2010 Reflexxes GmbH.
//  ----------------------------------------------------------
//   For a convenient reading of this file's source code,
//   please use a tab width of four characters.
//  ----------------------------------------------------------



#include <reflexxes_otg/TypeIOTGProfiles.h>
#include <reflexxes_otg/TypeIOTGMath.h>
#include <math.h>




//************************************************************************************
// ProfileStep1PosTri()

double TypeIOTGMath::ProfileStep1PosTri(		const double &CurrentPosition
											,	const double &CurrentVelocity
											,	const double &MaxAcceleration
											,	const double &TargetPosition)
{
	return (	(	2.0
				*	TypeIOTGMath::OTGSqrt(MaxAcceleration
				*	(TargetPosition - CurrentPosition) + 0.5
				*	pow2(CurrentVelocity)) - CurrentVelocity)
				/	MaxAcceleration);
}


//************************************************************************************
// ProfileStep1PosTrap()

double TypeIOTGMath::ProfileStep1PosTrap(		const double &CurrentPosition
											,	const double &CurrentVelocity
											,	const double &MaxVelocity
											,	const double &MaxAcceleration
											,	const double &TargetPosition)
{
	return (	(	TargetPosition - CurrentPosition)
				/	MaxVelocity + ( MaxVelocity
				-	CurrentVelocity + 0.5 * pow2(CurrentVelocity)
				/	MaxVelocity ) / MaxAcceleration);
}


//************************************************************************************
// ProfileStep2V_To_Vmax()

void TypeIOTGMath::ProfileStep2V_To_Vmax(		double 									*CurrentPosition
											,	double 									*CurrentVelocity
											,	const double							&MaxVelocity
											,	const double							&MaxAcceleration
											,	double									*ElapsedTime					
											,	const bool								&SignsAreInverted
											,	TypeIOTGMath::TypeIMotionPolynomials	*P					)
{
	double		TimeForThisSegment	=	0.0;

	TimeForThisSegment	=	fabs((*CurrentVelocity - MaxVelocity) / MaxAcceleration );

	P->PolynomialTimes[P->ValidPolynomials] = *ElapsedTime + TimeForThisSegment;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (0.5 * MaxAcceleration), -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, MaxAcceleration, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (-0.5 * MaxAcceleration), *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -MaxAcceleration, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				+=	0.5 * (pow2(*CurrentVelocity) - pow2(MaxVelocity)) / MaxAcceleration;
	*CurrentVelocity				=	MaxVelocity;
	*ElapsedTime					+=	TimeForThisSegment;


	return;
}


//************************************************************************************
// ProfileStep2V_To_Zero()

void TypeIOTGMath::ProfileStep2V_To_Zero(		double 									*CurrentPosition
											,	double 									*CurrentVelocity
											,	const double							&MaxAcceleration
											,	double									*ElapsedTime					
											,	const bool								&SignsAreInverted
											,	TypeIOTGMath::TypeIMotionPolynomials	*P					)
{
	double		TimeForThisSegment	=	0.0;

	TimeForThisSegment	=	fabs( *CurrentVelocity / MaxAcceleration );

	P->PolynomialTimes[P->ValidPolynomials] = *ElapsedTime + TimeForThisSegment;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (0.5 * MaxAcceleration), -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, MaxAcceleration, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (-0.5 * MaxAcceleration), *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -MaxAcceleration, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				+=	0.5 * pow2(*CurrentVelocity) / MaxAcceleration;
	*CurrentVelocity				=	0.0;

	*ElapsedTime					+=	TimeForThisSegment;


	return;
}


//************************************************************************************
// ProfileStep2PosTrap()

void TypeIOTGMath::ProfileStep2PosTrap(		double									*CurrentPosition
										,	double									*CurrentVelocity
										,	const double 							&MaxAcceleration
										,	const double 							&TargetPosition
										,	const double 							&SynchronizationTime
										,	double									*ElapsedTime					
										,	const bool								&SignsAreInverted
										,	TypeIOTGMath::TypeIMotionPolynomials	*P						)
{
	double			HoldVelocity	=	0.0
				,	Time2			=	0.0
				,	Time3			=	0.0
				,	Position2		=	0.0
				,	Position3		=	0.0;

	// Calculate all trajectory parameters

	HoldVelocity	=		0.5 * (MaxAcceleration * (SynchronizationTime - *ElapsedTime) + *CurrentVelocity)
						-	0.5 * TypeIOTGMath::OTGSqrt(4.0 * MaxAcceleration * (*CurrentPosition - TargetPosition)
						+	MaxAcceleration * (SynchronizationTime - *ElapsedTime)
						*	(MaxAcceleration * (SynchronizationTime - *ElapsedTime) + 2.0 * *CurrentVelocity)
						-	pow2(*CurrentVelocity));

	Time2			=	*ElapsedTime + (HoldVelocity - *CurrentVelocity) / MaxAcceleration;
	Time3			=	SynchronizationTime - HoldVelocity / MaxAcceleration;
	Position2		=	*CurrentPosition + 0.5 * (Time2 - *ElapsedTime) * (HoldVelocity + *CurrentVelocity);
	Position3		=	TargetPosition - 0.5 * (SynchronizationTime - Time3) * HoldVelocity;

	// Set up up polynomials

	// 1st segment

	P->PolynomialTimes[P->ValidPolynomials] = Time2;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (-0.5 * MaxAcceleration), -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -MaxAcceleration, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (0.5 * MaxAcceleration), *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, MaxAcceleration, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				=	Position2;
	*CurrentVelocity				=	HoldVelocity;
	*ElapsedTime					=	Time2;


	// 2nd segment

	P->PolynomialTimes[P->ValidPolynomials] = Time3;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				=	Position3;
	*CurrentVelocity				=	HoldVelocity;
	*ElapsedTime					=	Time3;


	// 3rd segment

	P->PolynomialTimes[P->ValidPolynomials] = SynchronizationTime;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (0.5 * MaxAcceleration), -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, MaxAcceleration, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (-0.5 * MaxAcceleration), *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -MaxAcceleration, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				=	TargetPosition;
	*CurrentVelocity				=	0.0;
	*ElapsedTime					=	SynchronizationTime;


	// Final segment

	P->PolynomialTimes[P->ValidPolynomials] = OTG_INFINITY;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, 0.0, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, 0.0, *ElapsedTime);
	}

	P->ValidPolynomials++;

	return;
}


//************************************************************************************
// ProfileStep2NegHldNegLin()

void TypeIOTGMath::ProfileStep2NegHldNegLin(		double 									*CurrentPosition
												,	double									*CurrentVelocity
												,	const double 							&MaxAcceleration
												,	const double 							&TargetPosition
												,	const double 							&SynchronizationTime
												,	double									*ElapsedTime					
												,	const bool								&SignsAreInverted
												,	TypeIOTGMath::TypeIMotionPolynomials	*P						)
{
	double			HoldVelocity	=	0.0
				,	Time2			=	0.0
				,	Time3			=	0.0
				,	Position2		=	0.0
				,	Position3		=	0.0
				,	Denominator		=	0.0;

	// Calculate all trajectory parameters

	Denominator		=		(2.0 * MaxAcceleration * (SynchronizationTime - *ElapsedTime)
						-	2.0 * *CurrentVelocity);

	if (fabs(Denominator) < OTG_DENOMINATOR_EPSILON)
	{
		HoldVelocity	=	*CurrentVelocity;
		Time2			=	*ElapsedTime;
		Time3			=	*ElapsedTime;
		Position2		=	*CurrentPosition;
		Position3		=	*CurrentPosition;
	}
	else
	{
		HoldVelocity	=		(2.0 * MaxAcceleration
							*	(TargetPosition - *CurrentPosition)
							-	pow2(*CurrentVelocity))
							/	Denominator;		

		Time2			=	*ElapsedTime + (*CurrentVelocity - HoldVelocity) / MaxAcceleration;
		Time3			=	SynchronizationTime - HoldVelocity / MaxAcceleration;
		Position2		=	*CurrentPosition + 0.5 * (Time2 - *ElapsedTime) * (HoldVelocity + *CurrentVelocity);
		Position3		=	TargetPosition - 0.5 * (SynchronizationTime - Time3) * HoldVelocity;
	}

	// Set up up polynomials

	// 1st segment

	P->PolynomialTimes[P->ValidPolynomials] = Time2;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (0.5 * MaxAcceleration), -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, MaxAcceleration, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (-0.5 * MaxAcceleration), *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -MaxAcceleration, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				=	Position2;
	*CurrentVelocity				=	HoldVelocity;
	*ElapsedTime					=	Time2;


	// 2nd segment

	P->PolynomialTimes[P->ValidPolynomials] = Time3;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				=	Position3;
	*CurrentVelocity				=	HoldVelocity;
	*ElapsedTime					=	Time3;


	// 3rd segment

	P->PolynomialTimes[P->ValidPolynomials] = SynchronizationTime;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (0.5 * MaxAcceleration), -*CurrentVelocity, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, MaxAcceleration, -*CurrentVelocity, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( (-0.5 * MaxAcceleration), *CurrentVelocity, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, -MaxAcceleration, *CurrentVelocity, *ElapsedTime);
	}

	P->ValidPolynomials++;

	*CurrentPosition				=	TargetPosition;
	*CurrentVelocity				=	0.0;
	*ElapsedTime					=	SynchronizationTime;


	// Final segment

	P->PolynomialTimes[P->ValidPolynomials] = OTG_INFINITY;

	if (SignsAreInverted)
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, -*CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, 0.0, *ElapsedTime);
	}
	else
	{
		P->PositionPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, *CurrentPosition, *ElapsedTime);
		P->VelocityPolynomial[P->ValidPolynomials].SetCoefficients( 0.0, 0.0, 0.0, *ElapsedTime);
	}

	P->ValidPolynomials++;

	return;
}
