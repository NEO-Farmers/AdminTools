modded class Construction
{
	void NEO_admin_build_readynext()
	{
		string part_name;
		ConstructionPart part;
		int i;
		
		if (!m_ConstructionParts)
		{
			Print("[NEO_ADMIN_BUILD] Construction::NEO_admin_build_readynext m_ConstructionParts was null");
			return;
		}
		
		for (i = 0; i < m_ConstructionParts.Count(); ++i)
		{
			part_name = m_ConstructionParts.GetKey(i);
			part = m_ConstructionParts.Get(part_name);
			
			if (!part)
			{
				continue; // don't think this is possible but just in case
			}
			
			if (!part.IsBuilt() && HasRequiredPart(part_name) && !HasMaterials(part_name) && !HasConflictPart(part_name))
			{
				// basing the following on HasMaterials
				string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
				string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "Materials";
		
				if ( GetGame().ConfigIsExisting( cfg_path ) )
				{
					int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
				
					for ( int j = 0; j < child_count; j++ )
					{
						string child_name;
						GetGame().ConfigGetChildName( cfg_path, j, child_name );
						
						//get type, quantity from material
						string material_path;
						string slot_name;
						string type;
						float quantity;
						material_path = cfg_path + " " + child_name + " " + "slot_name";
						GetGame().ConfigGetText( material_path, slot_name );
						material_path = cfg_path + " " + child_name + " " + "type";
						GetGame().ConfigGetText( material_path, type );
						material_path = cfg_path + " " + child_name + " " + "quantity";
						quantity = GetGame().ConfigGetFloat( material_path );
												
						//if the selected material (or its quantity) is not available
						ItemBase attachment = ItemBase.Cast( GetParent().FindAttachmentBySlotName( slot_name ) );
						if (!attachment)
						{
							GameInventory gi = GetParent().GetInventory();
							if (gi)
							{
								attachment = ItemBase.Cast(gi.CreateAttachment(type));
							}
						}
						if ( attachment && attachment.GetQuantity() < quantity )
						{
							attachment.SetQuantity(quantity);
						}
					}
				}
			}
		}
	}
}