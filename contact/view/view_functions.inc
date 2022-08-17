<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: view_functions.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file provides functions that are needed by the view        |
 | contact module.                                                              |
 +------------------------------------------------------------------------------+

*/


function companyResultsFound($searchContact)
{
	// Trim the empty elements out of all the arrays we have in our search object.

	$searchContact->search_company_flags = trim_array($searchContact->search_company_flags);
	$searchContact->search_company_yes_industry = trim_array($searchContact->search_company_yes_industry);
	$searchContact->search_company_no_industry = trim_array($searchContact->search_company_no_industry);
	$searchContact->search_company_size = trim_array($searchContact->search_company_size);
	$searchContact->search_company_status_status = trim_array($searchContact->search_company_status_status);
	$searchContact->search_company_type = trim_array($searchContact->search_company_type);
    $searchContact->search_company_status_department = trim_array($searchContact->search_company_status_department);
	$searchContact->search_department_yes_industry = trim_array($searchContact->search_department_yes_industry);
	$searchContact->search_department_no_industry = trim_array($searchContact->search_department_no_industry);
	$searchContact->search_department_size = trim_array($searchContact->search_department_size);

	/*
	 Let's begin forming the query.  The first part is to determine what exactly we are going to be
	 pulling out of the database.  We do this using a switch statement on the 6 display variables.
	*/

	$sql = ("
		SELECT DISTINCT ec.employer_id
		");

	/*
	 End of query1, now create query2, which designates which tables we will be drawing from, and
	 the start of the where clause.
	*/

	$query2 .= ("
		FROM employer_company AS ec
		LEFT JOIN country_list AS ecl ON ecl.country_id=ec.country_id
		LEFT JOIN company_flags_set AS cfs ON cfs.employer_id=ec.employer_id
		LEFT JOIN department_company_status AS dcs ON dcs.employer_id=ec.employer_id
		WHERE ec.employer_id IS NOT NULL
		AND ec.company_display
		");

	/*
	 Done with query2, now create query3, which is the conditional part
	 of the query.  All of the criteria is included in this part of the
	 query.
	*/

	/*
	 Now create the part of the query that filters our search results.
	*/

	if (trim($searchContact->search_company_name))
	{
        if ($searchContact->search_company_name_as == 'start' || !$searchContact->search_company_name_as)
        {
            $query3 .= ("
                AND ec.company_name LIKE '" . addslashes($searchContact->search_company_name) . "%'
                ");
        }
        elseif ($searchContact->search_company_name_as == 'containing')
        {
            $query3 .= ("
                AND ec.company_name LIKE '%" . addslashes($searchContact->search_company_name) . "%'
                ");
        }
	}

	if ($searchContact->search_company_status_status && $searchContact->search_company_status_department)
    {
        $temp_query = " ";
        for ($i = 0; $i < (sizeof($searchContact->search_company_status_department) - 1); $i++)
        {
            $temp_query .= "dcs.department_id='" . $searchContact->search_company_status_department[$i] . "' OR ";
        }
        $temp_query .= "dcs.department_id='" . $searchContact->search_company_status_department[$i] . "' ";


        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_status_status) - 1); $i++)
        {
            $query3 .= ("(( ".$temp_query.") AND (dcs.status_id='" . $searchContact->search_company_status_status[$i] . "')) OR ");
        }

        $query3 .= ("((".$temp_query.") AND (dcs.status_id='" . $searchContact->search_company_status_status[$i] . "' ))");

        $query3 .= (" )");
    }

	if (trim($searchContact->search_company_email))
	{
		if ($searchContact->search_company_email_as == 'start' || !$searchContact->search_company_email_as)
		{
			$query3 .= ("
				AND ec.email LIKE '" . addslashes($searchContact->search_company_email) . "%'
				");
		}
		elseif ($searchContact->search_company_email_as == 'containing')
		{
			$query3 .= ("
				AND ec.email LIKE '%" . addslashes($searchContact->search_company_email) . "%'
				");
		}
	}

	if (trim($searchContact->search_company_phone))
	{
		if ($searchContact->search_company_phone_as == 'start' || !$searchContact->search_company_email_as)
		{
			$query3 .= ("
				AND ec.phone LIKE '" . addslashes($searchContact->search_company_phone) . "%'
				");
		}
		elseif ($searchContact->search_company_phone_as == 'containing')
		{
			$query3 .= ("
				AND ec.phone LIKE '%" . addslashes($searchContact->search_company_phone) . "%'
				");
		}
	}

	if (trim($searchContact->search_company_name))
	{
		$query3 .= ("
			AND ec.company_name LIKE '" . addslashes($searchContact->search_company_name) . "%'
			");
	}

	if ($searchContact->search_company_yes_industry)
	{
		$query3 .= ("
			AND (
			");

		for ($i = 0; $i < (sizeof($searchContact->search_company_yes_industry) - 1); $i++)
		{
			$query3 .= ("
				ec.industry_id='" . addslashes($searchContact->search_company_yes_industry[$i]) . "' OR
				");
		}

		$query3 .= ("
			ec.industry_id='" . addslashes($searchContact->search_company_yes_industry[$i]) . "'
			");

		$query3 .= ("
			)
			");
	}

	if ($searchContact->search_company_no_industry)
	{
		$query3 .= ("
			AND NOT (
			");
		
		for ($i = 0; $i < (sizeof($searchContact->search_company_no_industry) - 1); $i++)
		{
			$query3 .= ("
				ec.industry_id='" . addslashes($searchContact->search_company_no_industry[$i]) . "' OR
				");
		}
			
		$query3 .= ("
			ec.industry_id='" . addslashes($searchContact->search_company_no_industry[$i]) . "'
			");
		
		$query3 .= ("
			)
			");
	}

	if ($searchContact->search_company_type)
    {
        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_type) - 1); $i++)
        {
            if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
            {
                $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR ec.company_type_id='0' OR ");
            }else{
                $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "' OR ");
            }
        }

        if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
        {
            $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR ec.company_type_id='0' ");
        }else{
            $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) ." '");
        }

        $query3 .= (" ) ");
    }

	if ($searchContact->search_company_size)
	{
        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_size) - 1); $i++)
        {
            if(strcmp("Unknown",$searchContact->search_company_size[$i]) == 0)
            {
                $query3 .= ("ec.size_id='' OR ec.size_id IS NULL OR  ec.size_id='7' OR ");
            }else{
                $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "' OR ");
            }
        }

        if(strcmp("Unknown",$searchContact->search_company_size[$i]) == 0)
        {
            $query3 .= ("(ec.size_id='' OR ec.size_id IS NULL or ec.size_id='7')");
        }else{
            $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "'");
        }

        $query3 .= (")");
    }

    if (trim($searchContact->search_company_area))
	{
		$query3 .= ("
			AND ecl.area_id='" . addslashes($searchContact->search_company_area) . "'
			");
	}

	if (trim($searchContact->search_company_country))
	{
		$query3 .= ("
			AND ec.country_id='" . addslashes($searchContact->search_company_country) . "'
			");
	}

	if (trim($searchContact->search_company_provstate))
	{
		$query3 .= ("
			AND ec.provstate_id='" . addslashes($searchContact->search_company_provstate) . "'
			");
	}

	if (trim($searchContact->search_company_region))
	{
		$query3 .= ("
			AND ec.region_id='" . addslashes($searchContact->search_company_region) . "'
			");
	}

	if (trim($searchContact->search_company_city))
	{
		$query3 .= ("
			AND ec.city LIKE '" . addslashes($searchContact->search_company_city) . "%'
			");
	}

	if (trim($searchContact->search_company_postal_code))
	{
		$query3 .= ("
			AND ec.postal_code LIKE '" . addslashes($searchContact->search_company_postal_code) . "%'
			");
	}

	if ($checked_flags)
	{
		if ($searchContact->search_company_flags_boolean == 'and' || !$searchContact->search_company_flags_boolean)
		{
			for ($i = 0; $i < sizeof($searchContact->search_company_flags); $i++)
			{
				$prequery = ("
					SELECT DISTINCT employer_id
					FROM company_flags_set
					WHERE flag_id='" . $searchContact->search_company_flags[$i] . "'
					");
				if (isset($employer_ids))
				{
					if (!$employer_ids)
					{
						$employer_ids = "''";
					}

					$prequery .= ("
						AND employer_id IN (" . $employer_ids . ")
						");
				}
				
				$preresult = $GLOBALS['dbh']->Execute($prequery);

				$employer_ids = '';
			
				while ($row = $preresult->FetchRow())
				{
					$employer_ids .= ("'" . $row["employer_id"] . "',");
				}
				$employer_ids = substr($employer_ids, 0, (strlen($employer_ids) - 1));
			}
			if (!$employer_ids)
			{
				$employer_ids = "''";
			}

			$query3 .= ("
				AND ec.employer_id IN (" . $employer_ids . ")
				");
		}
		elseif ($searchContact->search_company_flags_boolean == 'or')
		{
			$query3 .= ("
				AND cfs.flag_id IN (
				");

			for ($i = 0; $i < (sizeof($searchContact->search_company_flags) - 1); $i++)
			{
				$query3 .= ("
					'" . addslashes($searchContact->search_company_flags[$i]) . "',
					");
			}
			$query3 .= ("
				'" . addslashes($searchContact->search_company_flags[$i]) . "'
				");
			
			$query3 .= ("
				)
				");
		}
	}

	$sql .= $query2;

	$sql .= $query3;
	$result = $GLOBALS['dbh']->Execute($sql);

	return ($result->RecordCount());
}

function departmentResultsFound($searchContact)
{
	// Trim the empty elements out of all the arrays we have in our search object.

	$searchContact->search_company_yes_industry = trim_array($searchContact->search_company_yes_industry);
	$searchContact->search_company_no_industry = trim_array($searchContact->search_company_no_industry);
	$searchContact->search_company_size = trim_array($searchContact->search_company_size);
	$searchContact->search_company_status_status = trim_array($searchContact->search_company_status_status);
	$searchContact->search_company_type = trim_array($searchContact->search_company_type);
    $searchContact->search_company_status_department = trim_array($searchContact->search_company_status_department);
	$searchContact->search_department_yes_industry = trim_array($searchContact->search_department_yes_industry);
	$searchContact->search_department_no_industry = trim_array($searchContact->search_department_no_industry);
	$searchContact->search_department_size = trim_array($searchContact->search_department_size);


	/*
	 Let's begin forming the query.  The first part is to determine what exactly we are going to be
	 pulling out of the database.  We do this using a switch statement on the 6 display variables.
	*/

	$sql = ("
		SELECT DISTINCT ed.department_id
		");

	/*
	 End of query1, now create query2, which designates which tables we will be drawing from, and
	 the start of the where clause.
	*/

	$query2 .= ("
		FROM employer_department AS ed, employer_company AS ec
		LEFT JOIN country_list AS ecl ON ecl.country_id=ec.country_id
		LEFT JOIN country_list AS edl ON edl.country_id=ed.country_id
		LEFT JOIN department_division_status AS dds ON dds.division_id=ed.department_id
		WHERE ec.employer_id=ed.employer_id
		");

	/*
	 Done with query2, now create query3, which is the conditional part
	 of the query.  All of the criteria is included in this part of the
	 query.
	*/

	// Now create the part of the query that filters our search results.

	if (trim($searchContact->search_company_name))
	{
        if ($searchContact->search_company_name_as == 'start' || !$searchContact->search_company_name_as)
        {
            $query3 .= ("
                AND ed.department_name LIKE '" . addslashes($searchContact->search_company_name) . "%'
                ");
        }
        elseif ($searchContact->search_company_name_as == 'containing')
        {
            $query3 .= ("
                AND ed.department_name LIKE '%" . addslashes($searchContact->search_company_name) . "%'
                ");
        }
	}

	if ($searchContact->search_company_status_status && $searchContact->search_company_status_department)
	{
        $temp_query = " ";
        for ($i = 0; $i < (sizeof($searchContact->search_company_status_department) - 1); $i++)
        {
            $temp_query .= "dds.department_id='" . $searchContact->search_company_status_department[$i] . "' OR ";
        }

        $temp_query .= "dds.department_id='" . $searchContact->search_company_status_department[$i] . "' ";


        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_status_status) - 1); $i++)
        {
            $query3 .= ("(( ".$temp_query.") AND (dds.status_id='" . $searchContact->search_company_status_status[$i] . "')) OR ");
        }

        $query3 .= ("((".$temp_query.") AND (dds.status_id='" . $searchContact->search_company_status_status[$i] . "' ))");

        $query3 .= (" )");
    }

    if (trim($searchContact->search_company_website))
	{
		if ($searchContact->search_company_website_as == 'start' || !$searchContact->search_company_website_as)
		{
			$query3 .= ("
				AND ed.department_website LIKE '" . addslashes($searchContact->search_company_website) . "%'
				");
		}
		elseif ($searchContact->search_company_website_as == 'containing')
		{
			$query3 .= ("
				AND ed.department_website LIKE '%" . addslashes($searchContact->search_company_website) . "%'
				");
		}
	}

	if (trim($searchContact->search_company_email))
	{
		if ($searchContact->search_company_email_as == 'start' || !$searchContact->search_company_email_as)
		{
			$query3 .= ("
				AND ed.email LIKE '" . addslashes($searchContact->search_company_email) . "%'
				");
		}
		elseif ($searchContact->search_company_email_as == 'containing')
		{
			$query3 .= ("
				AND ed.email LIKE '%" . addslashes($searchContact->search_company_email) . "%'
				");
		}
	}

	if (trim($searchContact->search_company_phone))
	{
		if ($searchContact->search_company_phone_as == 'start' || !$searchContact->search_company_email_as)
		{
			$query3 .= ("
				AND ed.phone LIKE '" . addslashes($searchContact->search_company_phone) . "%'
				");
		}
		elseif ($searchContact->search_company_phone_as == 'containing')
		{
			$query3 .= ("
				AND ed.phone LIKE '%" . addslashes($searchContact->search_company_phone) . "%'
				");
		}
	}

	if ($searchContact->search_company_yes_industry)
	{
		$query3 .= ("
			AND (
			");

		for ($i = 0; $i < (sizeof($searchContact->search_company_yes_industry) - 1); $i++)
		{
			$query3 .= ("
				ed.industry_id='" . addslashes($searchContact->search_company_yes_industry[$i]) . "' OR
				");
		}

		$query3 .= ("
			ed.industry_id='" . addslashes($searchContact->search_company_yes_industry[$i]) . "'
			");

		$query3 .= ("
			)
			");
	}

	if ($searchContact->search_company_no_industry)
	{
		$query3 .= ("
			AND NOT(
			");

		for ($i = 0; $i < (sizeof($searchContact->search_company_no_industry) - 1); $i++)
		{
			$query3 .= ("
				ed.industry_id='" . addslashes($searchContact->search_company_no_industry[$i]) . "' OR
				");
		}

		$query3 .= ("
			ed.industry_id='" . addslashes($searchContact->search_company_no_industry[$i]) . "'
			");

		$query3 .= ("
			)
			");
	}

	if ($searchContact->search_company_type)
	{
        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_type) - 1); $i++)
        {
            if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
            {
                $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR ec.company_type_id=0'' OR");
            }else{
                $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "' OR ");
            }
        }

        if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
        {
            $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR ec.company_type_id='0'");
        }else{
            $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) ." '");
        }

        $query3 .= (" ) ");
    }

    if ($searchContact->search_company_size)
    {
        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_size) - 1); $i++)
        {
            if(strcmp("Unknown",$searchContact->search_company_size[$i]) == 0)
            {
                $query3 .= ("ec.size_id='' OR ec.size_id IS NULL OR  ec.size_id='7' OR ");
            }else{
                $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "' OR ");
            }
        }

        if(strcmp("Unknown",$searchContact->search_company_size[$i]) == 0)
        {
            $query3 .= ("(ec.size_id='' OR ec.size_id IS NULL or ec.size_id='7')");
        }else{
            $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "'");
        }

        $query3 .= (")");
    }

	if (trim($searchContact->search_company_area))
	{
		$query3 .= ("
			AND edl.area_id='" . addslashes($searchContact->search_company_area) . "'
			");
	}

	if (trim($searchContact->search_company_country))
	{
		$query3 .= ("
			AND ed.country_id='" . addslashes($searchContact->search_company_country) . "'
			");
	}

	if (trim($searchContact->search_company_provstate))
	{
		$query3 .= ("
			AND ed.provstate_id='" . addslashes($searchContact->search_company_provstate) . "'
			");
	}

	if (trim($searchContact->search_company_region))
	{
		$query3 .= ("
			AND ed.region_id='" . addslashes($searchContact->search_company_region) . "'
			");
	}

	if (trim($searchContact->search_company_city))
	{
		$query3 .= ("
			AND ed.city LIKE '" . addslashes($searchContact->search_company_city) . "%'
			");
	}

	if (trim($searchContact->search_company_postal_code))
	{
		$query3 .= ("
			AND ed.postal_code LIKE '" . addslashes($searchContact->search_company_postal_code) . "%'
			");
	}


	if ($search_company_criteria)
	{
		if (trim($searchContact->search_company_website))
		{
			if ($searchContact->search_company_website_as == 'start' || !$searchContact->search_company_website_as)
			{
				$query3 .= ("
					AND ec.website LIKE '" . addslashes($searchContact->search_company_website) . "%'
					");
			}
			elseif ($searchContact->search_company_website_as == 'containing')
			{
				$query3 .= ("
					AND ec.website LIKE '%" . addslashes($searchContact->search_company_website) . "%'
					");
			}
		} 

		if (trim($searchContact->search_company_email))
		{
			if ($searchContact->search_company_email_as == 'start' || !$searchContact->search_company_email_as)
			{
				$query3 .= ("
					AND ec.email LIKE '" . addslashes($searchContact->search_company_email) . "%'
					");
			}
			elseif ($searchContact->search_company_email_as == 'containing')
			{
				$query3 .= ("
					AND ec.email LIKE '%" . addslashes($searchContact->search_company_email) . "%'
					");
			}
		}

		if (trim($searchContact->search_company_phone))
		{
			if ($searchContact->search_company_phone_as == 'start' || !$searchContact->search_company_phone_as)
			{
				$query3 .= ("
					AND ec.phone LIKE '" . addslashes($searchContact->search_company_phone) . "%'
					");
			}
			elseif ($searchContact->search_company_phone_as == 'containing')
			{
				$query3 .= ("
					AND ec.phone LIKE '%" . addslashes($searchContact->search_company_phone) . "%'
					");
			}
		}

		if ($searchContact->search_company_yes_industry)
		{
			$query3 .= ("
				AND (
				");
			
			for ($i = 0; $i < (sizeof($searchContact->search_company_yes_industry) - 1); $i++)
			{
				$query3 .= ("
					ec.industry_id='" . addslashes($searchContact->search_company_yes_industry[$i]) . "' OR
					");
			}
			
			$query3 .= ("
				ec.industry_id='" . addslashes($searchContact->search_company_yes_industry[$i]) . "'
				");
			$query3 .= ("
				)
				");
		}

		if ($searchContact->search_company_no_industry)
		{
			$query3 .= ("
				AND NOT(
				");

			for ($i = 0; $i < (sizeof($searchContact->search_company_no_industry) - 1); $i++)
			{
				$query3 .= ("
					ec.industry_id='" . addslashes($searchContact->search_company_no_industry[$i]) . "' OR
					");
			}

			$query3 .= ("
				ec.industry_id='" . addslashes($searchContact->search_company_no_industry[$i]) . "'
				");
			$query3 .= ("
				)
				");
		}

		if ($searchContact->search_company_size)
        {
            $query3 .= (" AND ( ");

            for ($i = 0; $i < (sizeof($searchContact->search_company_size) - 1); $i++)
            {
                if(strcmp("Unknown",$searchContact->search_company_size[$i]) == 0)
                {
                    $query3 .= ("ec.size_id='' OR ec.size_id IS NULL OR  ec.size_id='7' OR ");
                }else{
                    $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "' OR ");
                }
            }

            if(strcmp("Unknown",$searchContact->search_company_size[$i]) == 0)
            {
                $query3 .= ("(ec.size_id='' OR ec.size_id IS NULL or ec.size_id='7')");
            }else{
                $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "'");
            }

            $query3 .= (")");
        }

        if (trim($searchContact->search_company_area))
		{        
			$query3 .= ("
				AND ecl.area_id='" . addslashes($searchContact->search_company_area) . "'
				");     
		}               
					
		if (trim($searchContact->search_company_country))
		{       
			$query3 .= ("
				AND ec.country_id='" . addslashes($searchContact->search_company_country) . "'
				");
		}               
					
		if (trim($searchContact->search_company_provstate))
		{
			$query3 .= ("
				AND ec.provstate_id='" . addslashes($searchContact->search_company_provstate) . "'
				");     
		}                               
						
		if (trim($searchContact->search_company_region))
		{
			$query3 .= ("
				AND ec.region_id='" . addslashes($searchContact->search_company_region) . "'
				");     
		}               
					
		if (trim($searchContact->search_company_city))
		{       
			$query3 .= ("
				AND ec.city LIKE '" . addslashes($searchContact->search_company_city) . "%'
				");
		}
		
		if (trim($searchContact->search_company_postal_code))
		{
			$query3 .= ("
				AND ec.postal_code LIKE '" . addslashes($searchContact->search_company_postal_code) . "%'
				");
		}
	}

	$sql .= $query2;

	$sql .= $query3;
    

	$result = $GLOBALS['dbh']->Execute($sql);

	return ($result->RecordCount());
}

?>
