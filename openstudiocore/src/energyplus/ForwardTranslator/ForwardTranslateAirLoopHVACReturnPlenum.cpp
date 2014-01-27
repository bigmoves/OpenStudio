/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/AirLoopHVACReturnPlenum.hpp>
#include <model/AirLoopHVACReturnPlenum_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <utilities/idd/AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACReturnPlenum( AirLoopHVACReturnPlenum & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  std::string s;

  IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ReturnPlenum);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setName(s);

  //((ZoneName)(Zone Name))
  //((ZoneNodeName)(Zone Node Name))
  //((InducedAirOutletNodeorNodeListName)(Induced Air Outlet Node or NodeList Name))

  // OutletNodeName

  if( boost::optional<model::ModelObject> node = modelObject.outletModelObject() )
  {
    idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName,node->name().get());
  }

  //// ZoneName
  //// and
  //// ZoneNodeName

  if( boost::optional<model::ThermalZone> zone = modelObject.thermalZone() )
  {
    if( boost::optional<IdfObject> _zone = translateAndMapModelObject(zone.get()) )
    {
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::ZoneName,_zone->name().get());

      model::Node node = zone->zoneAirNode();
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::ZoneNodeName,node.name().get());
    }
  }


  // InletNodeName
  
  std::vector<ModelObject> inletModelObjects = modelObject.inletModelObjects();
  for( std::vector<ModelObject>::iterator it = inletModelObjects.begin();
       it < inletModelObjects.end();
       it++ )
  {
    IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
    eg.setString(AirLoopHVAC_ReturnPlenumExtensibleFields::InletNodeName,it->name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio
