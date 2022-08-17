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
 | Filename: view_company_search_results.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is opened after the user has entered the search       |
 | criteria for viewing company.  The query is formed, performed on the         |
 | database, and then the results are displayed.                                |
 +------------------------------------------------------------------------------+

 //:TODO: Fix up this file's pagination. It's inefficient.
*/
?>

<script type="text/javascript" language="javascript">
<!--javascript
function email_submit(to){
    document.emailform1.to.value = to;
    document.emailform1.submit();
}
//-->
</script>

<?php
if(strcmp($message_sent,"yes")==0)
{
    echo "<br />".notify("The e-mail has been sent successfully.")."<br />";
    $message_sent == "";
}
// These variables will help reduce the problem of too many companies on a screen

if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
if ($start_row == '') { $start_row = 0; }

// Trim the empty elements out of all the arrays we have in our search object.

$searchContact->search_company_flags = trim_array($searchContact->search_company_flags);
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

$sql = ("SELECT DISTINCT ec.employer_id, ");

for ($i = 1; $i < 7; $i++)
{
    $varname = ("\$variable = \$searchContact->search_display" . $i . ";");
    eval($varname);

    switch($variable)
    {

        case "name":

            if (!$display_array || !in_array("ec.company_name", $display_array))
            {
                $query1 .= ("ec.company_name, ");
                $display_array[] = "ec.company_name";
                $exquery1 .= ("ec.company_name AS 'Company Name', ");
            }
        break;

        case "website":

            if (!$display_array || !in_array("ec.website", $display_array))
            {
                $query1 .= ("ec.website, ");
                $display_array[] = "ec.website";
                $exquery1 .= ("ec.website AS 'Website', ");
            }
        break;

        case "email":

            if (!$display_array || !in_array("ec.email", $display_array))
            {
                $query1 .= ("ec.email, ");
                $display_array[] = "ec.email";
                $exquery1 .= ("ec.email AS 'E-mail', ");
            }
        break;

        case "phone":

            if (!$display_array || !in_array("ec.phone", $display_array))
            {
                $query1 .= ("ec.phone, ");
                $display_array[] = "ec.phone";
                $exquery1 .= ("ec.phone AS 'Phone', ");
            }
        break;

        case "fax":

            if (!$display_array || !in_array("ec.fax", $display_array))
            {
                $query1 .= ("ec.fax, ");
                $display_array[] = "ec.fax";
                $exquery1 .= ("ec.fax AS 'Fax', ");
            }
        break;

        case "postal_code":

            if (!$display_array || !in_array("ec.postal_code", $display_array))
            {
                $query1 .= ("ec.postal_code, ");
                $display_array[] = "ec.postal_code";
                $exquery1 .= ("ec.postal_code AS 'Postal Code', ");
            }
        break;

        case "city":

            if (!$display_array || !in_array("ec.city", $display_array))
            {
                $query1 .= ("ec.city, ");
                $display_array[] = "ec.city";
                $exquery1 .= ("ec.city AS 'City', ");
            }
        break;

        case "country":

            if (!$display_array || !in_array("ec.country_id", $display_array))
            {
                $query1 .= ("ec.country_id, ");
                $display_array[] = "ec.country_id";
                $exquery1 .= ("ecl.country_name AS 'Country', ");
            }
        break;

        case "provstate":

            if (!$display_array || !in_array("ec.provstate_id", $display_array))
            {
                $query1 .= ("ec.provstate_id, ");
                $display_array[] = "ec.provstate_id";
                $exquery1 .= ("psl.provstate_name AS 'Prov/State', ");
            }
        break;

        case "region":

            if (!$display_array || !in_array("ec.region_id", $display_array))
            {
                $query1 .= ("ec.region_id, ");
                $display_array[] = "ec.region_id";
                $exquery1 .= ("rl.region_name AS 'Region', ");
            }
        break;

        case "address":

            if (!$display_array || !in_array("ec.street_address_1, ec.street_address_2", $display_array))
            {
                $query1 .= ("ec.street_address_1, ec.street_address_2, ");
                $display_array[] = "ec.street_address_1, ec.street_address_2";
                $exquery1 .= ("CONCAT(ec.street_address_1, ' ', ec.street_address_2) AS 'Address', ");
            }
        break;

        case "size":

            if (!$display_array || !in_array("ec.size_id", $display_array))
            {
                $query1 .= ("ec.size_id, ");
                $display_array[] = "ec.size_id";
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
        FROM employer_company AS ec
        LEFT JOIN country_list AS ecl ON ecl.country_id=ec.country_id
        LEFT JOIN company_flags_set AS cfs ON cfs.employer_id=ec.employer_id
        LEFT JOIN department_company_status AS dcs ON dcs.employer_id=ec.employer_id
        WHERE ec.employer_id IS NOT NULL
        AND ec.company_display
        ");

$exquery2 .= ("
        FROM employer_company AS ec
        LEFT JOIN country_list AS ecl ON ecl.country_id=ec.country_id
        LEFT JOIN provstate_list AS psl ON psl.provstate_id=ec.provstate_id
        LEFT JOIN region_list AS rl ON rl.region_id=ec.region_id
        LEFT JOIN employer_sizes AS es ON es.size_id=ec.size_id
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

// Now create the part of the query that filters our search results.

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

if ($searchContact->search_company_status_status)
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

    $query3 .= ("((".$temp_query.") AND (dcs.status_id='" . $searchContact->search_company_status_status[$i] . "' ) )");

    $query3 .= (" )");

    $query3 .= (" AND dcs.current_status = '1'");

}

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

if ($searchContact->search_company_size)
{
    $emptyarguments = "";
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

if ($searchContact->search_department_size)
{
    $emptyarguments = "";
    $query3 .= (" AND ( ");

    for ($i = 0; $i < (sizeof($searchContact->search_department_size) - 1); $i++)
    {
        if(strcmp("Unknown",$searchContact->search_department_size[$i]) == 0)
        {
            $query3 .= ("ed.size_id='' OR ed.size_id IS NULL OR  ed.size_id='7' OR ");
        }else{
            $query3 .= ("ed.size_id='" . addslashes($searchContact->search_department_size[$i]) . "' OR ");
        }
    }

    if(strcmp("Unknown",$searchContact->search_department_size[$i]) == 0)
    {
        $query3 .= ("ed.size_id='' OR ed.size_id IS NULL or ed.size_id='7'");
    }else{
        $query3 .= ("ed.size_id='" . addslashes($searchContact->search_department_size[$i]) . "'");
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
                    $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR ec.company_type_id='0' OR ");
                }else{
                    $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "' OR ");
                }
            }            

            if(strcmp("Unknown",$searchContact->search_company_type[$i]) == 0)
            {
                $query3 .= ("ec.company_type_id='' OR ec.company_type_id IS NULL OR  ec.company_type_id='0' ");
            }else{
                $query3 .= (" ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) ." '");
            }

            $query3 .= (" ) ");
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

if (sizeof($searchContact->search_company_flags) && is_array($searchContact->search_company_flags))
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

            if (!$order_array || !in_array("ec.company_name", $order_array))
            {
                $query4 .= ("ec.company_name, ");
                $order_array[] = "ec.company_name";
            }
        break;

        case "website":

            if (!$order_array || !in_array("ec.website", $order_array))
            {
                $query4 .= ("ec.website, ");
                $order_array[] = "ec.website";
            }
        break;

        case "email":

            if (!$order_array || !in_array("ec.email", $order_array))
            {
                $query4 .= ("ec.email, ");
                $order_array[] = "ec.email";
            }
        break;

        case "phone":

            if (!$order_array || !in_array("ec.phone", $order_array))
            {
                $query4 .= ("ec.phone, ");
                $order_array[] = "ec.phone";
            }
        break;

        case "fax":

            if (!$order_array || !in_array("ec.fax", $order_array))
            {
                $query4 .= ("ec.fax, ");
                $order_array[] = "ec.fax";
            }
        break;

        case "postal_code":

            if (!$order_array || !in_array("ec.postal_code", $order_array))
            {
                $query4 .= ("ec.postal_code, ");
                $order_array[] = "ec.postal_code";
            }
        break;

        case "city":

            if (!$order_array || !in_array("ec.city", $order_array))
            {
                $query4 .= ("ec.city, ");
                $order_array[] = "ec.city";
            }
        break;

        case "country":

            if (!$order_array || !in_array("ecl.order_by", $order_array))
            {
                $query4 .= ("ecl.order_by, ");
                $order_array[] = "ecl.order_by";
            }
        break;

        case "provstate":

            if (!$order_array || !in_array("ec.provstate_id", $order_array))
            {
                $query4 .= ("ec.provstate_id, ");
                $order_array[] = "ec.provstate_id";
            }
        break;

        case "region":

            if (!$order_array || !in_array("ec.region_id", $order_array))
            {
                $query4 .= ("ec.region_id, ");
                $order_array[] = "ec.region_id";
            }
        break;

        case "address":

            if (!$order_array || !in_array("ec.street_address_1, ec.street_address_2", $order_array))
            {
                $query4 .= ("ec.street_address_1, ec.street_address_2, ");
                $order_array[] = "ec.street_address_1, ec.street_address_2";
            }
        break;

        case "company":

            if (!$order_array || !in_array("ec.company_name", $order_array))
            {
                $query4 .= ("ec.company_name, ");
                $order_array[] = "ec.company_name";
            }
        break;

        //If the order var doesn't match any of these above cases, then do nothing.

    } //switch
} //for

if (trim($query4) == "ORDER BY")
{
    $query4 .= ("ec.company_name");
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
        SELECT DISTINCT ec.employer_id
        ");     

$sql_copy .= $query2;   
$sql_copy .= $query3;
$sql_copy .= $query4;

// ids_result is used to generate an array of the contact_ids returned by this search.

$ids_result = $GLOBALS['dbh']->Execute($sql_copy);

$result = $GLOBALS['dbh']->Execute($sql);

$count_result = $GLOBALS['dbh']->Execute($sql_copy);


$count = $count_result->RecordCount();
$pages = ceil($count / $per_page_max);

$first = $start_row + 1;
$end = $start_row + $per_page_max;
if ($end > $count) { $end = $count; }

echo("<form name='emailform1' method='post' action='".$PHP_SELF."'>");
echo("<input type='hidden' name='to' value='' />");
echo("<input type='hidden' name='employer_id_list' value='".$employer_id_list."' />");
echo ("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />");
echo("<input type='hidden' name='select' value='set_company_search_results_mail' />");
echo("</form>");

// Check if we have any matches for departments, and if so, set the variable to be true.

$departments_match = departmentResultsFound($searchContact);

if ($result->RecordCount() == 0)
{
    if ($departments_match)
    {
        echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
        echo("<input type='hidden' name='select' value='view_contact' />");
        echo("<input type='hidden' name='level1' value='company' />");
        echo("<input type='hidden' name='continue' value='view_division_search_results' />");
        echo("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");

        echo("</form>");

        ?>

        <script type="text/javascript" language="javascript">
        <!--javascript
            document.form1.submit();
            //-->
            </script>

            <?php
    }
    else
    {
        notify("There are currently no companies, nor departments in the database that match the criteria you have searched on.");
    }
}
elseif ($result->RecordCount() == 1)
{
    $row = $result->FetchRow();
    $employer_id = $row["employer_id"];
    $employer_id_list = $employer_id;

    echo("<form method='post' name='form1' action='" . $PHP_SELF . "'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='company' />");
    echo("<input type='hidden' name='continue' value='view_specific_company' />");
    echo("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
    echo("<input type='hidden' name='employer_id_list' value='" . $employer_id_list . "' />");
    echo("<input type='hidden' name='show_quick' value='true' />");
    echo("<input type='hidden' name='employer_id' value='" . $employer_id . "' />");
    echo("<input type='hidden' name='departments_match' value='" . $departments_match . "' />");

    echo("</form>");

    ?>

        <script type="text/javascript" language="javascript">
        <!--javascript
        document.form1.submit();
        //-->
        </script>

        <?php
}
else
{
    while ($ids_row = $ids_result->FetchRow())
    {       
        $employer_id_list .= $ids_row["employer_id"] . ",";
    }
    $employer_id_list = substr($employer_id_list, 0, (strlen($employer_id_list) - 1));

    if ($departments_match)
    {
        $msg .= ("There were divisions in the database that matched the criteria you searched on.  Click the button below to view these divisions.");
        $msg .= ("</b>"); //This is for xhtml compliancy since there are bold tags within the instruct() function
        $msg .= ("<br /><br />");
        $msg .= ("<form method='post' action='" . $PHP_SELF . "'>");
        $msg .= ("<input type='hidden' name='select' value='view_contact' />");
        $msg .= ("<input type='hidden' name='level1' value='company' />");
        $msg .= ("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
        $msg .= ("<input type='hidden' name='continue' value='view_division_search_results' />");
        $msg .= ("<center><input type='submit' value='View Divisions Returned' /></center>");
        $msg .= ("</form>");
       // $msg .= ("<b>&nbsp;"); //This is for xhtml compliancy since there are bold tags within the instruct() function

        instruct($msg);
    }

    echo("<h4 align='left'>Results of search (Companies):</h4>");

    echo("<form method='post' action='" . $PHP_SELF . "' name='main_form'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='company' />");
    echo("<input type='hidden' name='continue' value='view_specific_company' />");
    echo("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
    echo("<input type='hidden' name='employer_id_list' value='" . $employer_id_list . "' />");
    echo("<input type='hidden' name='show_quick' value='true' />");

    if ($count > $per_page_max)
    {
        echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

        echo("<tr>");
        echo("<td>");
        echo($count . " compan" . (($count != 1) ? "ies" : "y") . " on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");
        echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
        echo(" companies per page.");
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
            $company_start = getCompanyName($count_row[0]);

            if (strlen($company_start) > 15) 
            { 
                $company_start = substr($company_start, 0, 13) . "..."; 
            }

            $count_result->Move($row_num_end);
            $count_row = $count_result->FetchRow();
            $company_end = getCompanyName($count_row[0]);

            if (strlen($company_end) > 15) 
            { 
                $company_end = substr($company_end, 0, 13) . "..."; 
            }

            echo("<option value='" . $row_num_start . "' ");
            if ($start_row == $row_num_start)
            {
                echo("selected='selected'");
            }
            echo(">" . $company_start . " to " . $company_end . "</option>\n");
        }

        echo("</select>");
        echo("</td>");

        echo("<td align='right'>");
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

            case "ec.company_name":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Company");
            echo("</b></td>");
            break;

            case "ec.website":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Website");
            echo("</b></td>");
            break;

            case "ec.email":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("E-mail");
            echo("</b></td>");
            break;

            case "ec.phone":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Phone");
            echo("</b></td>");
            break;

            case "ec.fax":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Fax");
            echo("</b></td>");
            break;

            case "ec.postal_code":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Postal/Zip Code");
            echo("</b></td>");
            break;

            case "ec.country_id":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Country");
            echo("</b></td>");
            break;

            case "ec.provstate_id":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Province");
            echo("</b></td>");
            break;

            case "ec.region_id":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Region");
            echo("</b></td>");
            break;

            case "ec.city":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("City");
            echo("</b></td>");
            break;

            case "ec.street_address_1, ec.street_address_2":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Address");
            echo("</b></td>");
            break;

            case "ec.size_id":

                echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>");
            echo("Size");
            echo("</b></td>");
            break;

            //If the display var doesn't match any of these above cases, then do nothing.

        } //switch
    } //for

    echo("</tr>\n");

    $href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_company&amp;employer_id_list=" . $employer_id_list);

    $rowclass = 0;

    while ($row = $result->FetchRow())
    {
        echo("<tr>\n");
        echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("<input type='radio' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "' name='employer_id' value='" . $row["employer_id"] . "' onclick='submit()' />");
        echo("</td>");

        for ($i = 0; $i < sizeof($display_array); $i++)
        {
            echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;");

            switch($display_array[$i])
            {

                case "ec.company_name":

                    echo(($row["company_name"]) ? $row["company_name"] : "&nbsp;");
                break;

                case "ec.website":

                    echo("<a href='http://" . $row["website"] . "' target='company_website' class='blue'>");
                echo(($row["website"]) ? $row["website"] : "&nbsp;");
                echo("</a>");
                break;

                case "ec.email":
                    echo("<a href='javascript:email_submit(\"".$row["email"]."\")'>". $row["email"]. "</a>");
                    //echo(($row["email"]) ? "<a href='mailto:" . $row["email"] . "' class='blue'>" . $row["email"] . "</a>" : "&nbsp;");
                break;

                case "ec.phone":

                    echo(($row["phone"]) ? $row["phone"] : "&nbsp;");
                break;

                case "ec.fax":

                    echo(($row["fax"]) ? $row["fax"] : "&nbsp;");
                break;

                case "ec.postal_code":

                    echo(($row["postal_code"]) ? $row["postal_code"] : "&nbsp;");
                break;

                case "ec.country_id":

                    echo(($row["country_id"]) ? getCountryName($row["country_id"]) : "&nbsp;");
                break;

                case "ec.provstate_id":

                    echo(($row["provstate_id"]) ? getProvstateName($row["provstate_id"]) : "&nbsp;");
                break;

                case "ec.region_id":

                    echo(($row["region_id"]) ? getRegionName($row["region_id"]) : "&nbsp;");
                break;

                case "ec.city":

                    echo(($row["city"]) ? $row["city"] : "&nbsp;");
                break;

                case "ec.street_address_1, ec.street_address_2":

                    echo(($row["street_address_1"]) ? $row["street_address_1"] : "&nbsp;");
                if (trim($row["street_address_2"]))
                {
                    echo("<br />");
                    echo($row["street_address_2"]);
                }
                break;

                case "ec.size_id":

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
    echo("<input type='submit' value='View Selected Company' />");
    echo(" &nbsp; ");
    $export_string = rs2tab($GLOBALS['dbh']->Execute($export_sql));
    echo("<input type='hidden' name='export_string' value='". packObject($export_string) ."' />\n");
    echo("<input type='hidden' name='export' value='' />\n");
    echo("</td>\n");
    echo("</tr>\n");
    echo("</table>\n");

    echo("</td>\n");
    echo("</tr>\n");
    echo("</table>\n");


    echo("</form>\n");
    echo("<table>");

    echo("<tr>");
    echo("<td>");
    echo("<input type='button' value=' Export to File ' onclick='javascript:triggerExport(\"main_form\",\"export\",\" Export to File \")' /> ");
    echo("</td>");
    echo("</tr>");

    if (strlen($employer_id_list)) {
        // plugins
        // if you want to do anything else with the results, just use the following hook
        $arr_params = array();
        $arr_params['employer_id_list'] = $employer_id_list;
        $arr_params['PHP_SELF'] = $PHP_SELF;

        $arr_buttons = get_hooks('mamook.company_search_results.button', $arr_params);
        if (is_array($arr_buttons) && sizeof($arr_buttons)) {
            echo("<tr>");
            echo("<td>");
            echo("&nbsp;");
            echo("</td>");
            echo("</tr>");
            echo("<tr>");
            echo("<td>");
            for ($i = 0; $i < sizeof($arr_buttons); $i++) {
                echo($arr_buttons[$i]['button']).'&nbsp;';
            }
            echo("</td>");
            echo("</tr>");
        }
    }

    if(!$search_saved) {
        echo("<tr>");
        echo("<td>");
        echo("&nbsp;");
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td>");
        echo("<input type='button' value='Save Search' onclick='javascript:document.save_search_form.submit()' /> ");
    }
    echo("</td>");
    echo("</tr>");
    echo("</table>");

    if(!$search_saved)
    {
        echo("<form method='post' name='save_search_form' action='$PHP_SELF'>
                <input type='hidden' name='select' value='view_contact' />
                <input type='hidden' name='level1' value='company' />
	            <input type='hidden' name='continue' value='save_company_search' />
                <input type='hidden' name='btnSearch' value='Save Search' />
                <input type='hidden' name='searchContact' value='".packObject($searchContact)."' />
                <input type='hidden' name='search_comp_or_div' value='company' />
                </form>");
    }
    // form for generating a position posting/application record pdf report
    if (strlen($employer_id_list) > 0) 
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
