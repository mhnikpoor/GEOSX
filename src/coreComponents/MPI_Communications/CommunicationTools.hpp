/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2018, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * CommunicationTools.hpp
 *
 *  Created on: Jan 6, 2018
 *      Author: settgast
 */

#ifndef SRC_COMPONENTS_CORE_SRC_MPI_COMMUNICATIONS_COMMUNICATIONTOOLS_HPP_
#define SRC_COMPONENTS_CORE_SRC_MPI_COMMUNICATIONS_COMMUNICATIONTOOLS_HPP_

#include "common/DataTypes.hpp"
#include "mpi.h"
#include <set>
namespace geosx
{


class ObjectManagerBase;
class NeighborCommunicator;
class MeshLevel;

class MPI_iCommData;


class CommunicationTools
{
public:
  CommunicationTools();
  ~CommunicationTools();

  static void AssignGlobalIndices( ObjectManagerBase & object,
                                   ObjectManagerBase const & compositionObject,
                                   array1d<NeighborCommunicator> & neighbors );

  static void AssignNewGlobalIndices( ObjectManagerBase & object,
                                      set<localIndex> const & indexList );


  static void FindGhosts( MeshLevel * const meshLevel,
                          array1d<NeighborCommunicator> & neighbors );

  static int MPI_Size( MPI_Comm const & comm );
  static int MPI_Rank( MPI_Comm const & comm );

  static std::set<int> & getFreeCommIDs();
  static int reserveCommID();
  static void releaseCommID( int & ID );

  static void FindMatchedPartitionBoundaryObjects( ObjectManagerBase * const group,
                                                   array1d<NeighborCommunicator> & allNeighbors );

  static void SynchronizeFields( const std::map<string, string_array >& fieldNames,
                                 MeshLevel * const mesh,
                                 array1d<NeighborCommunicator> & allNeighbors );

  static void SynchronizePackSendRecvSizes( const std::map<string, string_array >& fieldNames,
                                            MeshLevel * const mesh,
                                            array1d<NeighborCommunicator> & neighbors,
                                            MPI_iCommData & icomm );

  static void SynchronizePackSendRecv( const std::map<string, string_array >& fieldNames,
                                       MeshLevel * const mesh,
                                       array1d<NeighborCommunicator> & allNeighbors,
                                       MPI_iCommData & icomm );

  static void SynchronizeUnpack( MeshLevel * const mesh,
                                 array1d<NeighborCommunicator> & neighbors,
                                 MPI_iCommData & icomm );

};


class MPI_iCommData
{
public:

  MPI_iCommData():
    size(0),
    commID(-1),
    sizeCommID(-1),
    fieldNames(),
    mpiSendBufferRequest(),
    mpiRecvBufferRequest(),
    mpiSendBufferStatus(),
    mpiRecvBufferStatus()
  {
    commID = CommunicationTools::reserveCommID();
    sizeCommID = CommunicationTools::reserveCommID();
  }

  ~MPI_iCommData()
  {
    if( commID >= 0 )
    {
      CommunicationTools::releaseCommID(commID);
    }

    if( sizeCommID >= 0 )
    {
      CommunicationTools::releaseCommID(sizeCommID);
    }

  }

  void resize( localIndex numMessages )
  {
    mpiSendBufferRequest.resize( numMessages );
    mpiRecvBufferRequest.resize( numMessages );
    mpiSendBufferStatus.resize( numMessages );
    mpiRecvBufferStatus.resize( numMessages );
    mpiSizeSendBufferRequest.resize( numMessages );
    mpiSizeRecvBufferRequest.resize( numMessages );
    mpiSizeSendBufferStatus.resize( numMessages );
    mpiSizeRecvBufferStatus.resize( numMessages );
    size = static_cast<int>(numMessages);
  }

  int size;
  int commID;
  int sizeCommID;
  std::map<string, string_array > fieldNames;

  array1d<MPI_Request> mpiSendBufferRequest;
  array1d<MPI_Request> mpiRecvBufferRequest;
  array1d<MPI_Status>  mpiSendBufferStatus;
  array1d<MPI_Status>  mpiRecvBufferStatus;

  array1d<MPI_Request> mpiSizeSendBufferRequest;
  array1d<MPI_Request> mpiSizeRecvBufferRequest;
  array1d<MPI_Status>  mpiSizeSendBufferStatus;
  array1d<MPI_Status>  mpiSizeRecvBufferStatus;
};

} /* namespace geosx */

#endif /* SRC_COMPONENTS_CORE_SRC_MPI_COMMUNICATIONS_COMMUNICATIONTOOLS_HPP_ */