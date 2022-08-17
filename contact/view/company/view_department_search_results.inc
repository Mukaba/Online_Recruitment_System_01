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
 | Filename: view_department_search_results.inc                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is opened after the user has entered the search       |
 | criteria for viewing departments.  The query is formed, performed on the     |
 | database, and then the results are displayed.                                |
 +------------------------------------------------------------------------------+

 //:TODO: Fix up this file's pagination. It's inefficient.
*/

?>

<script type="text/javascript" language="javascript">
<!--javascript
function email_submit(to){
    document.emailform2.to.value = to;
    document.emailform2.submit();
}
//-->
</script>

<?php
if(strcmp($message_sent,"yes") == 0)
{
    echo "<br />".notify("The e-mail has been sent successfully.")."<br />";
    $message_sent == "";
}

// These variables will help reduce the problem of too many divisions on a screen

if ($per_page_max == "") 
{ 
    $per_page_max = 50; 
}
if ($per_page_max < 5) 
{ 
    $per_page_max = 5; 
}
if ($start_row == '') 
{ 
    $start_row = 0; 
}

// Trim the empty elements out of all the arrays we have in our search object.

$searchContact->search_company_yes_industry = trim_array($searchContact->search_company_yes_industry);
$searchContact->search_company_no_industry = trim_array($searchContact->search_company_no_industry);
$searchContact->search_company_size = trim_array($searchContact->search_company_size);
$searchContact->search_company_type = trim_array($searchContact->search_company_type);
$searchContact->search_company_status_status = trim_array($searchContact->search_company_status_status);
$searchContact->search_company_status_department = trim_array($searchContact->search_company_status_department);
$searchContact->search_department_yes_industry = trim_array($searchContact->search_department_yes_industry);
$searchContact->search_department_no_industry = trim_array($searchContact->search_department_no_industry);
$searchContact->search_department_size = trim_array($searchContact->search_department_size);

//Checking for right input
if($searchContact->search_company_status_status && !$searchContact->search_company_status_department)
{
    error("You must select at least one Department if you have status set");
    include('contact/view/company/view_company_criteria.inc');
}
else
{
    include_once(PATH_ADODB . 'toexport.inc.php');


/*
 Let's begin forming the query.  The first part is to determine what exactly we are going to be
 pulling out of the database.  We do this using a switch statement on the 6 display variables.
*/

// There's a caveat about how this sql statement is built. Each column we append, we end with a comma and a space.
// After this select statement is built, the last two characters (the last comma and space) are stripped off. So,
// the format is important, although this should be changed at some point to something more robust. 

$sql = ("SELECT DISTINCT ed.department_id, ed.location_info, ec.employer_id, ");

for ($i = 1; $i < 7; $i++)
{
	$varname = ("\$variable = \$searchContact->search_display" . $i . ";");
	eval($varname);

	switch($variable)
	{
	
	case "name":

		if (!$display_array || !in_array("ed.department_name", $display_array))
		{
			$query1 .= ("ed.department_name, ec.company_name, ");
			$display_array[] = "ed.department_name";
            $display_array[] = "ec.company_name";
			$exquery1 .= ("ed.department_name AS 'Name', ");
            $exquery1 .= ("ec.company_name AS 'Company', ");
		}
		break;
	
	case "website":
		if (!$display_array || !in_array("ed.department_website", $display_array))
		{
			$query1 .= ("ed.department_website, ");
			$display_array[] = "ed.department_website";
			$exquery1 .= ("ed.department_website AS 'Website', ");
		}
		break;
	
	case "email":
		
		if (!$display_array || !in_array("ed.email", $display_array))
		{
			$query1 .= ("ed.email, ");
			$display_array[] = "ed.email";
			$exquery1 .= ("ed.email AS 'E-mail', ");
		}
		break;

	case "phone":
	
		if (!$display_array || !in_array("ed.phone", $display_array))
		{
			$query1 .= ("ed.phone, ");
			$display_array[] = "ed.phone";
			$exquery1 .= ("ed.phone AS 'Phone', ");
		}
		break;

	case "fax":
		
		if (!$display_array || !in_array("ed.fax", $display_array))
		{
			$query1 .= ("ed.fax, ");
			$display_array[] = "ed.fax";
			$exquery1 .= ("ed.fax AS 'Fax', ");
		}
		break;

	case "postal_code":
	
		if (!$display_array || !in_array("ed.postal_code", $display_array))
		{
			$query1 .= ("ed.postal_code, ");
			$display_array[] = "ed.postal_code";
			$exquery1 .= ("ed.postal_code AS 'Postal Code', ");
		}
		break;
	
	case "country":
	
		if (!$display_array || !in_array("ed.country_id", $display_array))
		{
			$query1 .= ("ed.country_id, ");
			$display_array[] = "ed.country_id";
			$exquery1 .= ("edl.country_name AS 'Country', ");
		}
		break;

	case "provstate":
	
		if (!$display_array || !in_array("ed.provstate_id", $display_array))
		{
			$query1 .= ("ed.provstate_id, ");
			$display_array[] = "ed.provstate_id";
			$exquery1 .= ("psl.provstate_name AS 'Prov/State', ");
		}
		break;

	case "region":

		if (!$display_array || !in_array("ed.region_id", $display_array))
		{
			$query1 .= ("ed.region_id, ");
			$display_array[] = "ed.region_id";
			$exquery1 .= ("rl.region_name AS 'Region', ");
		}
		break;

	case "city":

		if (!$display_array || !in_array("ed.city", $display_array))
		{
			$query1 .= ("ed.city, ");
			$display_array[] = "ed.city";
			$exquery1 .= ("ed.city AS 'City', ");
		}
		break;
	
	case "address":

		if (!$display_array || !in_array("ed.street_address_1, ed.street_address_2", $display_array))
		{
			$query1 .= ("ed.street_address_1, ed.street_address_2, ");
			$display_array[] = "ed.street_address_1, ed.street_address_2";
			$exquery1 .= ("CONCAT(ed.street_address_1, ' ', ed.street_address_2) AS 'Address', ");
		}
		break;
		
	case "company":
		
		if (!$display_array || !in_array("ec.company_name", $display_array))
		{
			$query1 .= ("ec.company_name, ");
			$display_array[] = "ec.company_name";
			$exquery1 .= ("ec.company_name AS 'Company Name', ");
		}
		break;

	case "size":
	
		if (!$display_array || !in_array("ed.size_id", $display_array))
		{
			$query1 .= ("ed.size_id, ");
			$display_array[] = "ed.size_id";
			$exquery1 .= ("es.size_range AS 'Size', ");
		}
		break;
	
	//If the display var doesn't match any of these above cases, then do nothing.

	} //switch
} //for

$query1 = substr($query1, 0, strlen($query1) - 2);
$exquery1 = substr($exquery1, 0, strlen($exquery1) - 2);

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

$exquery2 .= ("
	FROM employer_department AS ed, employer_company AS ec
	LEFT JOIN country_list AS ecl ON ecl.country_id=ec.country_id
	LEFT JOIN country_list AS edl ON edl.country_id=ed.country_id
	LEFT JOIN provstate_list AS psl ON psl.provstate_id=ed.provstate_id
	LEFT JOIN region_list AS rl ON rl.region_id=ed.region_id
	LEFT JOIN employer_sizes AS es ON es.size_id=ed.size_id
	LEFT JOIN company_flags_set AS cfs ON cfs.employer_id=ed.department_id
	LEFT JOIN department_division_status AS dds ON dds.division_id=ed.department_id
	WHERE ec.employer_id=ed.employer_id 
	");

/*
 Done with query2, now create query3, which is the conditional part
 of the query.  All of the criteria is included in this part of the
 query.
*/

/* 
 Now create the part of the query that filters our search results.  If we are searching solely based on an employer_id, then include that in the query,
 and ignore all other search criteria.
*/

if ($searchContact->search_using_only_employer_id)
{
    $query3 .= ("
        AND ec.employer_id='" . addslashes($searchContact->search_employer_id) . "'
        ");
}
else
{
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

    if ($searchContact->search_company_status_status)
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
        $query3 .= (" AND dds.current_status = '1'");

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
            $query3 .= ("ec.size_id='' OR ed.size_id IS NULL or ec.size_id='7'");
        }else{
            $query3 .= ("ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "'");
        }

        $query3 .= (")");        
    }

    if ($searchContact->search_company_type)
    {
        $query3 .= (" AND ( ");

        for ($i = 0; $i < (sizeof($searchContact->search_company_type) - 1); $i++)
        {
            if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
            {
                $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR  ec.company_type_id='0' OR ");
            }else{
                $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "' OR ");
            }
        }
        

        if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
        {
            $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR  ec.company_type_id='0'");
        }else{
            $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "'");
        }
        $query3 .= (" ) ");
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
            $query3 .= ("
                AND (
                ");

            for ($i = 0; $i < (sizeof($searchContact->search_company_size) - 1); $i++)
            {
                $query3 .= ("
                    ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "' OR
                    ");
            }
            
            $query3 .= ("
                ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "'
                ");
            $query3 .= ("
                )
                ");
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
}

/*
 End of query3.  Now forumlate query4, the ORDER BY part.
*/

$query4 = ("
	ORDER BY 
	");

for ($i = 1; $i < 5; $i++)
{
        $varname = ("\$variable = \$searchContact->search_order" . $i . ";");
        eval($varname);

        switch($variable)
        {

	case "name":

		if (!$order_array || !in_array("ed.department_name", $order_array))
		{
			$query4 .= ("ed.department_name, ");
			$order_array[] = "ed.department_name";
		}
		break;
	
	case "website":
		
		if (!$order_array || !in_array("ed.department_website", $order_array))
		{
			$query4 .= ("ed.department_website, ");
			$order_array[] = "ed.department_website";
		}
		break;
	
	case "email":
		
		if (!$order_array || !in_array("ed.email", $order_array))
		{
			$query4 .= ("ed.email, ");
			$order_array[] = "ed.email";
		}
		break;

	case "phone":
	
		if (!$order_array || !in_array("ed.phone", $order_array))
		{
			$query4 .= ("ed.phone, ");
			$order_array[] = "ed.phone";
		}
		break;

	case "fax":
		
		if (!$order_array || !in_array("ed.fax", $order_array))
		{
			$query4 .= ("ed.fax, ");
			$order_array[] = "ed.fax";
		}
		break;

	case "postal_code":
	
		if (!$order_array || !in_array("ed.postal_code", $order_array))
		{
			$query4 .= ("ed.postal_code, ");
			$order_array[] = "ed.postal_code";
		}
		break;
	
	case "country":
	
		if (!$order_array || !in_array("ed.country_id", $order_array))
		{
			$query4 .= ("ed.country_id, ");
			$order_array[] = "ed.country_id";
		}
		break;

	case "provstate":
	
		if (!$order_array || !in_array("ed.provstate_id", $order_array))
		{
			$query4 .= ("ed.provstate_id, ");
			$order_array[] = "ed.provstate_id";
		}
		break;

	case "region":

		if (!$order_array || !in_array("ed.region_id", $order_array))
		{
			$query4 .= ("ed.region_id, ");
			$order_array[] = "ed.region_id";
		}
		break;

	case "city":

		if (!$order_array || !in_array("ed.city", $order_array))
		{
			$query4 .= ("ed.city, ");
			$order_array[] = "ed.city";
		}
		break;
	
	case "address":

		if (!$order_array || !in_array("ed.street_address_1, ed.street_address_2", $order_array))
		{
			$query4 .= ("ed.street_address_1, ed.street_address_2, ");
			$order_array[] = "ed.street_address_1, ed.street_address_2";
		}
		break;
		
	case "company":
		
		if (!$order_array || !in_array("ec.company_name", $order_array))
		{

			$order_array[] = "ec.company_name";
		}
		break;

        //If the order var doesn't match any of these above cases, then do nothing.

        } //switch
} //for

if (trim($query4) == "ORDER BY")
{
	$query4 .= ("ed.department_name, ec.company_name");
}
else
{
	$query4 = substr($query4, 0, strlen($query4) - 2);
}

$query5 = ("
        LIMIT " . $start_row . ", " . $per_page_max
        );

$sql .= $query1;

$sql .= $query2;

$sql .= $query3;

$sql .= $query4;

$sql .= $query5;

// used for exporting to a file
$export_sql = ("
	SELECT DISTINCT
	");

$export_sql .= $exquery1;
$export_sql .= $exquery2;
$export_sql .= $query3;
$export_sql .= $query4;


$sql_copy = ("
	SELECT DISTINCT ed.department_id
        ");

$sql_copy .= $query2;
$sql_copy .= $query3;
$sql_copy .= $query4;

// ids_result is used to generate an array of the contact_ids returned by this search.
$ids_result = $GLOBALS['dbh']->Execute($sql_copy);
$result = $GLOBALS['dbh']->Execute($sql);
//:TODO when being passed a company name, that is being applied to the department search.  We need to change this, so that it is possible to view all divisions under
// a company

$count_result = $GLOBALS['dbh']->Execute($sql_copy);

$count = $count_result->RecordCount();
$pages = ceil($count / $per_page_max);

$first = $start_row + 1;
$end = $start_row + $per_page_max;
if ($end > $count) { $end = $count; }

//Email form
echo("<form name='emailform2' method='post' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='to' value='' />");
echo("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />");
echo("<input type='hidden' name='select' value='set_department_search_results_mail' />");
echo("</form>");

// Check if we have any matches for companies, and if so, set the variable to be true.

if (companyResultsFound($searchContact))
{
	$companies_match = TRUE;
}

if ($result->RecordCount() == 0)
{
	notify("There are currently no divisions in the database that match the criteria you have searched on.");

	if ($companies_match)
	{
		$msg .= ("There were companies in the database that matched the criteria you searched on.  Click the button below to view these companies.");
		$msg .= ("</b>"); //This is for xhtml compliancy since there are bold tags within the instruct() function
		$msg .= ("<br /><br />");
		$msg .= ("<form method='post' action='" . $PHP_SELF . "'>");
		$msg .= ("<input type='hidden' name='select' value='view_contact' />");
		$msg .= ("<input type='hidden' name='level1' value='company' />");
		$msg .= ("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
		$msg .= ("<input type='hidden' name='continue' value='view_company_search_results' />");
		$msg .= ("<center><input type='submit' value='View Companies Returned' /></center>");
		$msg .= ("</form>");
		$msg .= ("<b>&nbsp;"); //This is for xhtml compliancy since there are bold tags within the instruct() function
		
		instruct($msg);
	}
}
else
{
	while ($ids_row = $ids_result->FetchRow())
        {
                $department_id_list .= $ids_row["department_id"] . ",";
        }
        $department_id_list = substr($department_id_list, 0, (strlen($department_id_list) - 1));

	if ($companies_match)
	{
		$msg .= ("There were companies in the database that matched the criteria you searched on.  Click the button below to view these companies.");
		$msg .= ("</b>"); //This is for xhtml compliancy since there are bold tags within the instruct() function
		$msg .= ("<br /><br />");
		$msg .= ("<form method='post' action='" . $PHP_SELF . "'>");
		$msg .= ("<input type='hidden' name='select' value='view_contact' />");
		$msg .= ("<input type='hidden' name='level1' value='company' />");
		$msg .= ("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
		$msg .= ("<input type='hidden' name='continue' value='view_company_search_results' />");
		$msg .= ("<center><input type='submit' value='View Companies Returned' /></center>");
		$msg .= ("</form>");
		$msg .= ("<b>&nbsp;"); //This is for xhtml compliancy since there are bold tags within the instruct() function
		
		instruct($msg);
	}

	echo("<h4 align='left'>Results of search (Divisions):</h4>");

    echo("<form method='post' action='" . $PHP_SELF . "' name='main_form'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='company' />");
    echo("<input type='hidden' name='continue' value='view_specific_department' />");
    echo("<input type='hidden' name='show_more' value='departments' />");
    echo("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
    echo("<input type='hidden' name='department_id_list' value='" . $department_id_list . "' />");
    echo("<input type='hidden' name='show_quick' value='true' />");

    if ($count > $per_page_max) 
    {
        echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

            echo("<tr>");
            echo("<td>");
            echo($count . " division" . (($count != 1) ? "s" : "") . " on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
            echo(" divisions per page.");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");

            echo("<td>");
            echo("<select name='start_row'>");

            for ($i = 0; $i < $pages; $i++)
            {
                    $row_num_start = $i * $per_page_max;
                    $row_num_end = $row_num_start + $per_page_max - 1;
                    if ($row_num_end >= $count) $row_num_end = $count - 1;

                    $count_result->Move($row_num_start);
                    $count_row = $count_result->FetchRow();
                    $division_start = getDivisionName($count_row[0]);
                    if (strlen($division_start) > 15) { $division_start = substr($division_start, 0, 13) . "..."; }

                    $count_result->Move($row_num_end);
                    $count_row = $count_result->FetchRow();
                    $division_end = getDivisionName($count_row[0]);
                    if (strlen($division_end) > 15) { $division_end = substr($division_end, 0, 13) . "..."; }

                    echo("<option value='" . $row_num_start . "' ");
                    if ($start_row == $row_num_start)
                    {
                            echo("selected='selected'");
                    }
                    echo(">" . $division_start . " to " . $division_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td align=right>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
            echo("<br />");
   }

	echo("<table cellspacing='0' cellpadding='0' border='1'>");
	echo("<tr>");
		echo("<td>");
        	echo("<table border='0' cellpadding='2'>");
       		echo("<tr>");
			echo("<td class='rowgrey' align='center'>&nbsp;</td>");

		for ($i = 0; $i < sizeof($display_array); $i++)
		{

		        switch($display_array[$i])
		        {
		
		        case "ed.department_name":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
				echo("Division");
				echo("</b></td>");
		                break;
		
		        case "ed.department_website":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
				echo("Website");
				echo("</b></td>");
				break;
		
		        case "ed.email":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                	        echo("E-mail");
		                echo("</b></td>");
		                break;
		
		        case "ed.phone":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
        	                echo("Phone");
	                        echo("</b></td>");
		                break;
		
		        case "ed.fax":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
        	                echo("Fax");
	                        echo("</b></td>");
		                break;
			
		        case "ed.postal_code":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("Postal/Zip Code");
                                echo("</b></td>");
		                break;
		
		        case "ed.country_id":

				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("Country");
                                echo("</b></td>");
		                break;
		
		        case "ed.provstate_id":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("Province");
                                echo("</b></td>");
		                break;
		
		        case "ed.region_id":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("Region");
                                echo("</b></td>");
				break;
		
		        case "ed.city":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("City");
                                echo("</b></td>");
				break;
		
			case "ed.street_address_1, ed.street_address_2":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("Address");
                                echo("</b></td>");
				break;
		
		        case "ec.company_name":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
                                echo("Company");
                                echo("</b></td>");
		                break;

			case "ed.size_id":
		
				echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
				echo("Size");
				echo("</b></td>");
				break;
		
		        //If the display var doesn't match any of these above cases, then do nothing.
		
		        } //switch
		} //for

	echo("</tr>\n");

	$rowclass = 0;

	while ($row = $result->FetchRow())
	{
		/*
                 If this division is using the company's location, pull it out of the database.
                */

                if ($row["location_info"] == USE_COMPANY)
                {
                        $sql = ("
                                SELECT DISTINCT street_address_1, street_address_2, country_id, provstate_id, region_id, city, postal_code
                                FROM employer_company
                                WHERE employer_id='" . $row["employer_id"] . "'
                                ");
                        $location_result = $GLOBALS['dbh']->Execute($sql);
                        $location_row = $location_result->FetchRow();
                }

		echo("<tr>\n");
		echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
                echo("<input type='radio' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "' name='department_id' value='" . $row["department_id"] . "' onclick='submit()' />");
                echo("</td>");

		for ($i = 0; $i < sizeof($display_array); $i++)
		{
			echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
	                echo("&nbsp;");

		        switch($display_array[$i])
		        {
		
		        case "ed.department_name":
		
				echo(($row["department_name"]) ? $row["department_name"] : "&nbsp;");
		                break;
		
		        case "ed.department_website":
		
				echo("<a href='http://" . $row["department_website"] . "' target='department_website' class='blue'>");
				echo(($row["department_website"]) ? $row["department_website"] : "&nbsp;");
				echo("</a>");
		        break;
		
                case "ed.email":
                    echo("<a href='javascript:email_submit(\"".$row["email"]."\")'>". $row["email"]. "</a>");
                    //echo(($row["email"]) ? "<a href='mailto:" . $row["email"] . "' class='blue'>" . $row["email"] . "</a>" : "&nbsp;");
                break;

		        case "ed.phone":
		
				if ($row["phone"] == USE_COMPANY)
				{
					$sql = ("
						SELECT DISTINCT a.phone
						FROM employer_company AS a, employer_department AS b
						WHERE a.employer_id=b.employer_id
						AND b.department_id='" . $row["department_id"] . "'
						");
					$phone_result = $GLOBALS['dbh']->Execute($sql);
					$phone_row = $phone_result->FetchRow();
					echo(($phone_row["phone"]) ? $phone_row["phone"] : "&nbsp;");
				}
				else
				{
					echo(($row["phone"]) ? $row["phone"] : "&nbsp;");
				}
		                break;
		
		        case "ed.fax":
		
				if ($row["fax"] == USE_COMPANY)
                                {
                                        $sql = ("
                                                SELECT DISTINCT a.fax
                                                FROM employer_company AS a, employer_department AS b
                                                WHERE a.employer_id=b.employer_id
                                                AND b.department_id='" . $row["department_id"] . "'
                                                ");
                                        $fax_result = $GLOBALS['dbh']->Execute($sql);
                                        $fax_row = $fax_result->FetchRow();
                                        echo(($fax_row["fax"]) ? $fax_row["fax"] : "&nbsp;");
                                }
                                else
                                {
                                        echo(($row["fax"]) ? $row["fax"] : "&nbsp;");
                                }
		                break;
			
		        case "ed.postal_code":
		
				if ($row["location_info"] == USE_COMPANY)
				{
					echo(($location_row["postal_code"]) ? $location_row["postal_code"] : "&nbsp;");
				}
				else
				{
					echo(($row["postal_code"]) ? $row["postal_code"] : "&nbsp;");
				}
		                break;
		
		        case "ed.country_id":
		
				if ($row["location_info"] == USE_COMPANY)
				{
					echo(($location_row["country_id"]) ? getCountryName($location_row["country_id"]) : "&nbsp;");
				}
				else
				{
					echo(($row["country_id"]) ? getCountryName($row["country_id"]) : "&nbsp;");
				}
		                break;
		
		        case "ed.provstate_id":
		
				if ($row["location_info"] == USE_COMPANY)
				{
					echo(($location_row["provstate_id"]) ? getProvstateName($location_row["provstate_id"]) : "&nbsp;");
				}
				else
				{
					echo(($row["provstate_id"]) ? getProvstateName($row["provstate_id"]) : "&nbsp;");
				}
		                break;
		
		        case "ed.region_id":
		
				if ($row["location_info"] == USE_COMPANY)
				{
					echo(($location_row["region_id"]) ? getRegionName($location_row["region_id"]) : "&nbsp;");
				}
				else
				{
                                	echo(($row["region_id"]) ? getRegionName($row["region_id"]) : "&nbsp;");
				}
		                break;
		
		        case "ed.city":
		
				if ($row["location_info"] == USE_COMPANY)
				{
					echo(($location_row["city"]) ? $location_row["city"] : "&nbsp;");
				}
				else
				{
                                	echo(($row["city"]) ? $row["city"] : "&nbsp;");
				}
				break;
		
			case "ed.street_address_1, ed.street_address_2":
		
				if ($row["location_info"] == USE_COMPANY)
				{
					echo(($location_row["street_address_1"]) ? $location_row["street_address_1"] : "&nbsp;");
					if (trim($location_row["street_address_2"]))
					{
						echo("<br />");
						echo($location_row["street_address_2"]);
					}
				}
				else
				{
                                	echo(($row["street_address_1"]) ? $row["street_address_1"] : "&nbsp;");
					if (trim($row["street_address_2"]))
					{
						echo("<br />");
						echo($row["street_address_2"]);
					}
				}
		                break;
		
		        case "ec.company_name":
		
                                echo(($row["company_name"]) ? $row["company_name"] : "&nbsp;");
		                break;
		
			case "ed.size_id":
			
				echo(($row["size_id"]) ? getSizeRange($row["size_id"]) : "&nbsp;");
				break;

		        //If the display var doesn't match any of these above cases, then do nothing.
		
		        } //switch
			
			echo("</td>\n");

		} //for

		echo("</tr>\n");
		$rowclass++;
	}

	echo("<tr>");
        echo("<td colspan='" . (sizeof($display_array) + 1) . "' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("<input type='submit' value='View Selected Division' />");
        $export_string = rs2tab($GLOBALS['dbh']->Execute($export_sql));
        echo("<input type='hidden' name='export_string' value='". packObject($export_string) ."' />\n");
        echo("<input type='hidden' name='export' value='' />");
        echo("</td>");
    echo("</tr>");
	echo("</table>\n");

	echo("</td>\n");
	echo("</tr>\n");
	echo("</table>\n");
    echo("</form>\n");
    
    echo("<input type='button' value=' Export to File ' onclick='javascript:triggerExport(\"main_form\",\"export\",\" Export to File \")' /> ");
    echo("<br /><br />");

    if (strlen($department_id_list)) {
        // plugins
        // if you want to do anything else with the results, just use the following hook
        $arr_params = array();
        $arr_params['department_id_list'] = $department_id_list;
        $arr_params['PHP_SELF'] = $PHP_SELF;

        $arr_buttons = get_hooks('mamook.division_search_results.button', $arr_params);
        if (is_array($arr_buttons) && sizeof($arr_buttons)) {
            for ($i = 0; $i < sizeof($arr_buttons); $i++) {
                echo($arr_buttons[$i]['button']).'&nbsp;';
            }
        }
    }

    // form for generating a position posting/application record pdf report
    if (strlen($department_id_list))
    {
        // display the forms of the buttons from the plugins need
        if (is_array($arr_buttons) && sizeof($arr_buttons)) {
            for ($i = 0; $i < sizeof($arr_buttons); $i++) {
                echo($arr_buttons[$i]['form']);
            }
        }
    }
    ?>
        <script type='text/javascript'>
        <!--javascript

        function triggerExport(form_name, element_name, element_value) {

            form = eval("document." + form_name);
            flag = 0;
            var i;
            for (i = 0; i < form.elements.length; i++) {
                if (form.elements[i].name == element_name) {
                    form.elements[i].value = element_value;
                    flag = 1;
                    break;
                }
            }

            if (flag == 1) {
                form.submit();
                form.elements[i].value = null;
            }
        }

        //-->
        </script>

    <?php
}
}
?>
