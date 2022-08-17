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
 | Filename: view_contact_search_results.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is opened after the user has entered the search       |
 | criteria for viewing contacts.  The query is formed, performed on the        |
 | database, and then the results are displayed.                                |
 +------------------------------------------------------------------------------+

*/

include('contact/view/contact/display_array.inc');

if(strcmp($message_sent,"yes")==0)
{
    echo "<br />".notify("The e-mail has been sent successfully.")."<br />";
    $message_sent == "**";
}

// If a user uses dynamic ordering in the search results, we delete the the existing search order array and place the new
// ordering into the array. 
if ($neworder)
{
    $searchContact->search_order = NULL;
    $searchContact->search_order[1] = $neworder;
}

$sql = ("
    SELECT DISTINCT c.contact_id,
    ");

$selected_columns = array(); // this array keeps track of what columns have been selected
$join_array = array();       // this array creates a list of all values in search_display and search_order
                             // for later

// if no columns were selected to be displayed, we set default columns then.
if (sizeof($searchContact->search_display) <= 0 || !is_array($searchContact->search_display))
{
    for($i=0;$i<sizeof($display_array);$i++)
    {
        switch($display_array[$i]["name"])
        {
            case "Contact First Name":
                $contact_first_name_index = $i;
                break;
            case "Contact Last Name":
                $contact_last_name_index = $i;
                break;
            case "Contact Phone Number":
                $contact_phone_index = $i;
                break;
            case "Contact Cell Number":
                $contact_cellphone_index = $i;
                break;
            case "Contact E-mail":
                $contact_email_index = $i;
                break;
            case "Company Name":
                $company_name_index = $i;
                break;
            case "Division Name":
                $division_name_index = $i;
                break;
        }
    }

    $searchContact->search_display[1] = $contact_first_name_index;
    $searchContact->search_display[2] = $contact_last_name_index;
    $searchContact->search_display[3] = $contact_phone_index;
    $searchContact->search_display[4] = $contact_cellphone_index;
    $searchContact->search_display[5] = $contact_email_index;
    $searchContact->search_display[6] = $company_name_index;
    $searchContact->search_display[7] = $division_name_index;
}

// Set default order by statements if the user has not specified any. 
if (sizeof($searchContact->search_order) <= 0 || !is_array($searchContact->search_order))
{
    if (!$contact_first_name_index || !$contact_last_name_index)
    {
        for($i=0;$i<sizeof($display_array);$i++)
        {
            switch($display_array[$i]["name"])
            {
                case "Contact First Name":
                    $contact_first_name_index = $i;
                    break;
                case "Contact Last Name":
                    $contact_last_name_index = $i;
                    break;
            }
        }
    }

    $searchContact->search_order[1] = $contact_last_name_index;
    $searchContact->search_order[2] = $contact_first_name_index;
}

// begin forming SELECT statement.
if (sizeof($searchContact->search_display) > 0 && is_array($searchContact->search_display))
{
    foreach($searchContact->search_display as $display_value)
    {
        if ($display_value)
        {
            // piggy backing this array traversal for efficiency. joined_array is used later
            $join_array[] = $display_value;
            
            if ($display_value < 0 || $display_value > sizeof($display_array)-1)
            {
                notify("An error has occurred. The columns you have selected are out of bounds, please contact administrator");
                $error_flag = 1;
                break;
            }

            if ($display_array[$display_value]["name"] == "Contact Flags")
            {
                // The user has chosen to display contact flags, so we'll use this much later below. Contact flags are queried differently. We have to wait till
                // we get a list of contact ids, and for each contact id, pull back all flags associated with them. We can't do this in MySQL 3.x because nested
                // queries aren't supported. 
            }
            // Can't addslashes to display_array select fields that contain IF statements.
            // If we do, the query will break. 
            elseif (preg_match("/^(\s)*IF/",$display_array[$display_value]["select"]))
            {
                $sql .= $display_array[$display_value]["select"].",";
            }
            else
            {
                $sql .= addslashes($display_array[$display_value]["select"]).",";
            }
            $selected_columns[] = $display_value;
        }
        unset($display_value);
    }
}

// In some instances, if we want to order a paticular field, we have to select the column first before we can order it. For instance, 
// location information for contacts require this. Every contact has a dynamic address which can be stored in the contact table, 
// employer_deparmtent table, and employer_company table. I use IF statements to fetch the correct address information. So, to order
// this dynamic address infromation, one has to select these columns first using if statements.
if (sizeof($searchContact->search_order) > 0 && is_array($searchContact->search_order))
{
    foreach($searchContact->search_order as $order_value)
    {
        if ($order_value)
        {
            $join_array[] = $order_value;
            
            if ($order_value < 0 || $order_value > sizeof($display_array)-1)
            {
                notify("An error has occured. The ordering columns you have selected are out of bounds, please contact administrator");
                $error_flag = 1;
                break;
            }
            
            // in the display_array.inc file, the array has a 'select_required' attribute. This indicates whether if a
            // select statement is required to order this column. 
            elseif($display_array[$order_value]["select_required"] == 1)
            {
                $matched_flag = 0;
                foreach($selected_columns as $already_selected)
                {
                    if ($order_value == $already_selected)
                    {
                        $matched_flag = 1;
                        break;
                    }
                }

                // If we haven't already selected this column, select it now.
                if (!$matched_flag)
                {
                    if (preg_match("/^(\s)*IF/",$display_array[$order_value]["select"]))
                    {
                        $sql .= $display_array[$order_value]["select"].",";
                    }
                    else
                    {
                        $sql .= addslashes($display_array[$order_value]["select"]).",";
                    }
                    $selected_columns[] = $order_value;
                }
            }
        }
        $matched_flag = 0;
        unset($order_value);
    }
}

$search_contact_country_index = NULL;
$search_contact_provstate_index = NULL;
$search_contact_region_index = NULL;
$search_contact_city_index = NULL;
$search_contact_postal_code_index = NULL;
$search_contact_phone_index = NULL;
$search_contact_cellphone_index = NULL;
$search_contact_fax_index = NULL;
$search_department_country_index = NULL;
$search_department_provstate_index = NULL;
$search_department_region_index = NULL;
$search_department_city_index = NULL;
$search_department_postal_code_index = NULL;
$search_department_phone_index = NULL;
$search_department_fax_index = NULL;

for ($i=0;$i<sizeof($display_array);$i++)
{
    switch ($display_array[$i]["name"])
    {
        case "Contact Country":
            $search_contact_country_index = $i;
            break;
        case "Contact Province":
            $search_contact_provstate_index = $i;
            break;
        case "Contact Region":
            $search_contact_region_index = $i;
            break;
        case "Contact City":
            $search_contact_city_index = $i;
            break;
        case "Contact Postal Code":
            $search_contact_postal_code_index = $i;
            break;
        case "Contact Phone Number":
            $search_contact_phone_index = $i;
            break;
        case "Contact Cell Number":
            $search_contact_cellphone_index = $i;
            break;
        case "Contact Fax Number":
            $search_contact_fax_index = $i;
            break;
        case "Division Country":
            $search_department_country_index = $i;
            break;
        case "Division Province":
            $search_department_provstate_index = $i;
            break;
        case "Division Region":
            $search_department_region_index = $i;
            break;
        case "Division City":
            $search_department_city_index = $i;
            break;
        case "Division Postal Code":
            $search_department_postal_code_index = $i;
            break;
        case "Division Phone Number":
            $search_department_phone_index = $i;
            break;
        case "Division Fax Number":
            $search_department_fax_index = $i;
            break;
    }
}

// This next section of if statements will check if a user has used an location search criteria in their search. If so,
// these criteria will need to be added to the select statement. 
if ($searchContact->search_contact_area)
{
    $area_string = ("
        IF (c.location_info = '".USE_COMPANY."'
            , eccl.area_id
            , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , eccl.area_id
                , IF (c.location_info = '".USE_DEPARTMENT."'
                    , edcl.area_id
                    , ccl.area_id)))
        AS c_area_id
        ");
                                                                        
    $sql .= $area_string.",";
}

// The following if statements will ensure that no columns will be redundantly selected in a
// query. 
if ($searchContact->search_contact_country && strlen($search_contact_country_index))
{
    $matched_flag = 0;
    
    // Go through array and see if contact country has already been selected. 
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_country_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    // Contact country has not already been selected, add to the $selected_columns array to
    // indicate the column has been selected. Add contact country to the SELECT statement in SQL.
    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_country_index]["select"].",";
        $selected_columns[] = $search_contact_country_index;
    }
    unset($select_value);
}

if ($searchContact->search_contact_provstate && strlen($search_contact_provstate_index))
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_provstate_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_provstate_index]["select"].",";
        $selected_columns[] = $search_contact_provstate_index;
    }
    unset($select_value);
}

if ($searchContact->search_contact_region && strlen($search_contact_region_index))
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_region_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_region_index]["select"].",";
        $selected_columns[] = $search_contact_region_index;
    }
    unset($select_value);
}

if ($searchContact->search_contact_city && strlen($search_contact_city_index))
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_city_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_city_index]["select"].",";
        $selected_columns[] = $search_contact_city_index;
    }
    unset($select_value);
}

if (strlen(trim($searchContact->search_contact_postal_code)) && strlen($search_contact_postal_code_index))
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_postal_code_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_postal_code_index]["select"].",";
        $selected_columns[] = $search_contact_postal_code_index;
    }
    unset($select_value);
}

if (trim($searchContact->search_contact_phone_as) && strlen(trim($searchContact->search_contact_phone)) 
    && strlen($search_contact_phone_index)
    )
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_phone_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_phone_index]["select"].",";
        $selected_columns[] = $search_contact_phone_index;
    }
    unset($select_value);
}

if (trim($searchContact->search_contact_cellphone_as) && strlen(trim($searchContact->search_contact_cellphone)) 
    && strlen($search_contact_cellphone_index)
    )
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_cellphone_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_cellphone_index]["select"].",";
        $selected_columns[] = $search_contact_cellphone_index;
    }
    unset($select_value);
}
if (trim($searchContact->search_contact_fax_as) && strlen(trim($searchContact->search_contact_fax)) 
    && strlen($search_contact_fax_index)
    )
{
    $matched_flag = 0;
    foreach ($selected_columns as $select_value)
    {
        if ($select_value == $search_contact_fax_index)
        {
            $matched_flag = 1;
            break;
        }
    }

    if ($matched_flag == 0)
    {
        $sql .= $display_array[$search_contact_fax_index]["select"].",";
        $selected_columns[] = $search_contact_fax_index;
    }
    unset($select_value);
}

// If the user has division criteria in the search, then the following if statement executes.
// otherwise skip this block of code. The code contained here is very similar to the block of
// code above. 
if ($searchContact->search_department_criteria)
{
    if ($searchContact->search_department_area)
    {
        $area_string = ("
            IF (ed.location_info = '".USE_COMPANY."'
                , eccl.area_id
                , edcl.area_id
                )
            AS ed_area_id
            ");
        $sql .= $area_string.",";
    }


    if ($searchContact->search_department_country && $search_department_country_index)
    {
        $matched_flag = 0;
        
        // This foreach loop ensures that we don't redundantly select columns
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_country_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_country_index]["select"].",";
            // every time we select a column, we add it to the selected_columns array to indicate this column has been selected,
            // don't select this again. 
            $selected_columns[] = $search_department_country_index;
        }
        unset($select_value);
    }

    if ($searchContact->search_department_provstate && $search_department_provstate_index)
    {
        $matched_flag = 0;
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_provstate_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_provstate_index]["select"].",";
            $selected_columns[] = $search_department_provstate_index;
        }
        unset($select_value);
    }

    if ($searchContact->search_department_region && $search_department_region_index)
    {
        $matched_flag = 0;
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_region_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_region_index]["select"].",";
            $selected_columns[] = $search_department_region_index;
        }
        unset($select_value);
    }

    if ($searchContact->search_department_city && strlen($search_department_city_index))
    {
        $matched_flag = 0;
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_city_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_city_index]["select"].",";
            $selected_columns[] = $search_department_city_index;
        }
        unset($select_value);
    }

    if (strlen(trim($searchContact->search_department_postal_code)) && strlen($search_department_postal_code_index))
    {
        $matched_flag = 0;
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_postal_code_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_postal_code_index]["select"].",";
            $selected_columns[] = $search_department_postal_code_index;
        }
        unset($select_value);
    }

    if (trim($searchContact->search_department_phone_as) && strlen(trim($searchContact->search_department_phone)) 
        && strlen($search_department_phone_index)
        )
    {
        $matched_flag = 0;
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_phone_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_phone_index]["select"].",";
            $selected_columns[] = $search_department_phone_index;
        }
        unset($select_value);
    }

    if (trim($searchContact->search_department_fax_as) && strlen(trim($searchContact->search_department_fax)) 
        && strlen($search_department_fax_index)
        )
    {
        $matched_flag = 0;
        foreach ($selected_columns as $select_value)
        {
            if ($select_value == $search_department_fax_index)
            {
                $matched_flag = 1;
                break;
            }
        }

        if ($matched_flag == 0)
        {
            $sql .= $display_array[$search_department_fax_index]["select"].",";
            $selected_columns[] = $search_department_fax_index;
        }
        unset($select_value);
    }
}

$sql = trim($sql);
if (substr($sql,-1)==",")
{
    $sql = substr($sql,0,strlen($sql)-1);
}

// Finished selecting all the columns we need, begin joining tables that we need. 

$sql .= ("
    FROM contact c
    INNER JOIN contact_employer ce
    ON c.contact_id = ce.contact_id
    LEFT JOIN employer_department ed
    ON ce.department_id = ed.department_id
    LEFT JOIN employer_company ec
    ON ed.employer_id = ec.employer_id
    ");

// These flags indicate if a certain table has already been joined. This prevents duplicate joining.

$ccl_flag = 0;     // contact's country_list table.
$cpl_flag = 0;     // contact's provstate_list table.
$crl_flag = 0;     // contact's region_list table.

$ctitle_flag = 0;  // contact's title table
$ceis_flag = 0;    // contact's status table (employer_info_statuses)
$my_ceis_flag = 0; // contact's status table (employer_info_statuses) which brings back the department's status.
$dcs_flag = 0;     // contact's status (department_contact_status)

$edcl_flag = 0;  
$edpl_flag = 0;
$edrl_flag = 0;
$edes_flag = 0;    // employer_department's employer_sizes table.
$edind_flag = 0;   // employer_department's industries table.

$eccl_flag = 0;    // employer_company's country_list tables. 
$ecpl_flag = 0;   
$ecrl_flag = 0;
$eces_flag = 0;
$ecind_flag = 0;
$ecct_flag = 0;    // employer_company's company_type table

$cam_flag = 0;     // contact action methods table
$cat_flag = 0;     // contact action type table
$cas_flag = 0;     // contact action set table
$cab_c_flag = 0;   // contact action set table, we're pulling Contact Action By

// join_array contains a list of every column selected or used for ordering. Each value in join_array is an integer that
// maps to an array position in display_array.inc. We step through every element of this array and pull out a keyword 
// from display_array[$i]["select"] or display_array[$i]["order"]. Each relevent keyword is mapped to the switch statement
// below. Only certain order/select statements require a join statement.
foreach ($join_array as $select_column)
{
    // if the SELECT attribute of display_array contains an IF statement, we use this array position's order attribute
    // because it's easier to map to a case below. 
    if (preg_match("/IF/",$display_array[$select_column]["select"]))
    {
        $switch_key = $display_array[$select_column]["order"];

        // Two columns that are exceptions. Their select clauses look like this:
        // IF (cas.action_on = '0000-00-00', NULL, cas.action_on) AS cas_action_on
        // Which are non-standard, so this is a work around. 
        if ($switch_key == "cas.bring_forward_date" || $switch_key == "cas.action_on")
        {
            $switch_key = "cas";
        }
    }

    // otherwise, all select statements should follow the following convention alias.column name. This regular expression
    // uses the alias to match a case below. For example, if I was using ed.department_name in my select statement, I would
    // extract the ed, and match it to a case statement below. The statement will know that ed represents the employer_department
    // table, and needs to join with that table if it already hasn't been joined. 
    elseif (preg_match("/^(\w+)\..*/",$display_array[$select_column]["select"]))
    {
        $switch_key = preg_replace("/^(\w+)\..*/","\\1",$display_array[$select_column]["select"]);
    }

    if (trim($switch_key))
    {
        switch($switch_key)
        {
            case "c_country_name":
                if (!$ccl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list ccl
                        ON c.country_id = ccl.country_id
                        ");
                    $ccl_flag = 1;
                }
                
                if (!$edcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list edcl
                        ON ed.country_id = edcl.country_id
                        ");
                    $edcl_flag = 1;
                }
                
                if (!$eccl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list eccl
                        ON ec.country_id = eccl.country_id
                        ");
                    $eccl_flag = 1;
                }

                break;
                
            case "c_provstate_name":

                if (!$cpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list cpl
                        ON c.provstate_id = cpl.provstate_id
                        ");
                    $cpl_flag = 1;
                }

                if (!$edpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list edpl
                        ON ed.provstate_id = edpl.provstate_id
                        ");
                    $edpl_flag = 1;
                }

                if (!$ecpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list ecpl
                        ON ec.provstate_id = ecpl.provstate_id
                        ");
                    $ecpl_flag = 1;
                }
                
                break;

            case "c_region_name":

                if (!$crl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list crl
                        ON c.region_id = crl.region_id
                        ");
                    $crl_flag = 1;
                }

                if (!$edrl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list edrl
                        ON ed.region_id = edrl.region_id
                        ");
                    $edrl_flag = 1;
                }
                
                if (!$ecrl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list ecrl
                        ON ec.region_id = ecrl.region_id
                        ");
                    $ecrl_flag = 1;
                }

                break;

            case "ceis":

                if (!$ceis_flag)
                {
                    $sql .= ("
                        LEFT JOIN department_contact_status dcs
                        ON ce.contact_id = dcs.contact_id
                        LEFT JOIN employer_info_statuses ceis
                        ON dcs.status_id = ceis.status_id
                        ");
                    $ceis_flag = 1;
                }
                break;
                
            case "my_ceis":

                if (!$my_ceis_flag)
                {
                    $sql .= ("
                        LEFT JOIN department_contact_status my_dcs
                        ON ce.contact_id = my_dcs.contact_id AND my_dcs.current_status = '1' AND my_dcs.department_id = '".$auth->department."'
                        LEFT JOIN employer_info_statuses my_ceis
                        ON my_dcs.status_id = my_ceis.status_id
                        ");
                    $my_ceis_flag = 1;
                }
                break;

            case "ctitle":

                if (!$ctitle_flag)
                {
                    $sql .= ("
                        LEFT JOIN title ctitle
                        ON c.title = ctitle.title_id
                        ");
                    $ctitle_flag = 1;
                }
                break;

            case "ed_country_name":

                if (!$edcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list edcl
                        ON ed.country_id = edcl.country_id
                        ");
                    $edcl_flag = 1;
                }
                
                if (!$eccl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list eccl
                        ON ec.country_id = eccl.country_id
                        ");
                    $eccl_flag = 1;
                }

                break;
                
            case "ed_provstate_name":

                if (!$edpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list edpl
                        ON ed.provstate_id = edpl.provstate_id
                        ");
                    $edpl_flag = 1;
                }

                if (!$ecpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list ecpl
                        ON ec.provstate_id = ecpl.provstate_id
                        ");
                    $ecpl_flag = 1;
                }
                
                break;

            case "ed_region_name":

                if (!$edrl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list edrl
                        ON ed.region_id = edrl.region_id
                        ");
                    $edrl_flag = 1;
                }
                
                if (!$ecrl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list ecrl
                        ON ec.region_id = ecrl.region_id
                        ");
                    $ecrl_flag = 1;
                }

                break;

            case "edes":
                if (!$edes_flag)
                {
                    $sql .= ("
                        LEFT JOIN employer_sizes edes
                        ON ed.size_id = edes.size_id
                        ");
                    $edes_flag = 1;
                }
                break;

            case "edind":
                if (!$edind_flag)
                {
                    $sql .= ("
                        LEFT JOIN industries edind
                        ON ed.industry_id = edind.industry_id
                        ");
                    $edind_flag = 1;
                }
                break;

            case "eccl":
                if (!$eccl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list eccl
                        ON ec.country_id = eccl.country_id
                        ");
                    $eccl_flag = 1;
                }
                break;

            case "ecpl":
                if (!$ecpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list ecpl
                        ON ec.provstate_id = ecpl.provstate_id
                        ");
                    $ecpl_flag = 1;
                }
                break;
                
            case "ecrl":
                if (!$ecrl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list ecrl
                        ON ec.region_id = ecrl.region_id
                        ");
                    $ecrl_flag = 1;
                }
                break;
                
            case "eces":
                if (!$eces_flag)
                {
                    $sql .= ("
                        LEFT JOIN employer_sizes eces
                        ON ec.size_id = eces.size_id
                        ");
                    $eces_flag = 1;
                }
                break;

            case "ecind":
                if (!$ecind_flag)
                {
                    $sql .= ("
                        LEFT JOIN industries ecind
                        ON ec.industry_id = ecind.industry_id
                        ");
                    $ecind_flag = 1;
                }
                break;

            case "ecct":
                if (!$ecct_flag)
                {
                    $sql .= ("
                        LEFT JOIN company_type ecct
                        ON ec.company_type_id = ecct.type_id
                        ");
                    $ecct_flag = 1;
                }
                break;

            case "cam":
                if (!$cas_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact_actions_set cas
                        ON ce.contact_id = cas.contact_id
                        ");
                    $cas_flag = 1;
                }
                if (!$cam_flag)
                {
                    $sql .= ("
                        LEFT JOIN action_methods cam
                        ON cas.action_method_id = cam.action_method_id
                        ");
                    $cam_flag = 1;
                }
                break;

            case "cas":
                if (!$cas_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact_actions_set cas
                        ON ce.contact_id = cas.contact_id
                        ");
                    $cas_flag = 1;
                }
                break;

            case "cat":
                if (!$cas_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact_actions_set cas
                        ON ce.contact_id = cas.contact_id
                        ");
                    $cas_flag = 1;
                }
                if (!$cat_flag)
                {
                    $sql .= ("
                        LEFT JOIN action_types cat
                        ON cas.action_id = cat.action_id
                        ");
                    $cat_flag = 1;
                }
                break;

            case "cab_c":
                if (!$cas_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact_actions_set cas
                        ON ce.contact_id = cas.contact_id
                        ");
                    $cas_flag = 1;
                }
                if (!$cab_c_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact cab_c
                        ON cas.action_by = cab_c.contact_id
                        ");
                    $cab_c_flag = 1;
                }
                break;
        } // end switch
    }// end if
    unset($switch_key);
}// end foreach

unset($switch_key);

if (!$ceis_flag && (sizeof($searchContact->search_contact_status_status) > 0 && is_array($searchContact->search_contact_status_status)
    && sizeof($searchContact->search_contact_status_department) > 0 
    && is_array($searchContact->search_contact_status_department))
   )
{
    $sql .= ("
        LEFT JOIN department_contact_status dcs
        ON ce.contact_id = dcs.contact_id
        ");

}

// Finished dynamically joining tables from fields that the user has selected or has ordered by. Now we dynamically join
// fields of criteria they have selected. For example, if they want all contacts that are from Canada, the code below
// will join with the proper table to get that country information. 

if ($searchContact->search_contact_area || $searchContact->search_contact_country)
{
    if (!$ccl_flag)
    {
        $sql .= ("
            LEFT JOIN country_list ccl
            ON c.country_id = ccl.country_id
            ");
        $ccl_flag = 1;
    }

    if (!$edcl_flag)
    {
        $sql .= ("
            LEFT JOIN country_list edcl
            ON ed.country_id = edcl.country_id
            ");
        $edcl_flag = 1;
    }
    
    if (!$eccl_flag)
    {
        $sql .= ("
            LEFT JOIN country_list eccl
            ON ec.country_id = eccl.country_id
            ");
        $eccl_flag = 1;
    }
}

if ($searchContact->search_contact_provstate)
{
    if (!$cpl_flag)
    {
        $sql .= ("
            LEFT JOIN provstate_list cpl
            ON c.provstate_id = cpl.provstate_id
            ");
        $cpl_flag = 1;
    }
    
    if (!$edpl_flag)
    {
        $sql .= ("
            LEFT JOIN provstate_list edpl
            ON ed.provstate_id = edpl.provstate_id
            ");
        $edpl_flag = 1;
    }

    if (!$ecpl_flag)
    {
        $sql .= ("
            LEFT JOIN provstate_list ecpl
            ON ec.provstate_id = ecpl.provstate_id
            ");
        $ecpl_flag = 1;
    }
}

if ($searchContact->search_contact_region)
{
    if (!$crl_flag)
    {
        $sql .= ("
            LEFT JOIN region_list crl
            ON c.region_id = crl.region_id
            ");
        $crl_flag = 1;
    }

    if (!$edrl_flag)
    {
        $sql .= ("
            LEFT JOIN region_list edrl
            ON ed.region_id = edrl.region_id
            ");
        $edrl_flag = 1;
    }
    
    if (!$ecrl_flag)
    {
        $sql .= ("
            LEFT JOIN region_list ecrl
            ON ec.region_id = ecrl.region_id
            ");
        $ecrl_flag = 1;
    }
}

if (is_array($searchContact->search_contact_flags) && sizeof($searchContact->search_contact_flags) > 0 && strlen($searchContact->search_contact_flags_boolean))
{
    if ($searchContact->search_contact_flags_boolean == "and")
    {
        $i = 0;
        foreach ($searchContact->search_contact_flags as $value)
        {
            // An example of what this looks like after the loop is complete. (Assume user searches for 3 flags)

            // LEFT JOIN contact_flags_set cfs1
            // ON ce.contact_id = cfs1.contact_id         // we first link up with contact_employer table
            // LEFT JOIN contact_flags_set cfs2
            // ON cfs1.contact_id = cfs2.contact_id       // then we link the first cointact_flags_set table with the second, etc.
            // LEFT JOIN contact_flags_set cfs3
            // ON cfs2.contact_id = cfs3.contact_id
            $next = $i+1;
            if ($i == 0)
            {
                $sql .= ("
                    LEFT JOIN contact_flags_set cfs".$next."
                    ON ce.contact_id = cfs".$next.".contact_id
                    ");
            }

            else
            {
                $sql .= ("
                    LEFT JOIN contact_flags_set cfs".$next."
                    ON cfs".$i.".contact_id = cfs".$next.".contact_id
                    ");
            }
            $i++;
        }
    }

    elseif ($searchContact->search_contact_flags_boolean == "or")
    {
        $sql .= ("
            LEFT JOIN contact_flags_set cfs
            ON ce.contact_id = cfs.contact_id
            ");
    }
}

if (!$cas_flag && ($searchContact->search_contact_action || $searchContact->search_contact_action_by || ($searchContact->search_contact_action_as 
    && $searchContact->search_contact_action_on) || $searchContact->search_contact_action_between1 || $searchContact->search_contact_action_between2
    || ($searchContact->search_contact_action_bf_as && $searchContact->search_contact_action_bf_on) || $searchContact->search_contact_action_bf_between1 
    || $searchContact->search_contact_action_bf_between2)
   )
{
    $sql .= ("
        LEFT JOIN contact_actions_set cas
        ON ce.contact_id = cas.contact_id
        ");
    $cas_flag = 1;
}

if ($searchContact->search_department_criteria)
{
    if ($searchContact->search_department_area || $searchContact->search_department_country)
    {
        if (!$edcl_flag)
        {
            $sql .= ("
                LEFT JOIN country_list edcl
                ON ed.country_id = edcl.country_id
                ");
            $edcl_flag = 1;
        }
        
        if (!$eccl_flag)
        {
            $sql .= ("
                LEFT JOIN country_list eccl
                ON ec.country_id = eccl.country_id
                ");
            $eccl_flag = 1;
        }
    }

    if ($searchContact->search_department_provstate)
    {
        if (!$edpl_flag)
        {
            $sql .= ("
                LEFT JOIN provstate_list edpl
                ON ed.provstate_id = edpl.provstate_id
                ");
            $edpl_flag = 1;
        }

        if (!$ecpl_flag)
        {
            $sql .= ("
                LEFT JOIN provstate_list ecpl
                ON ec.provstate_id = ecpl.provstate_id
                ");
            $ecpl_flag = 1;
        }
    }

    if ($searchContact->search_department_region)
    {
        if (!$edrl_flag)
        {
            $sql .= ("
                LEFT JOIN region_list edrl
                ON ed.region_id = edrl.region_id
                ");
            $edrl_flag = 1;
        }
        
        if (!$ecrl_flag)
        {
            $sql .= ("
                LEFT JOIN region_list ecrl
                ON ec.region_id = ecrl.region_id
                ");
            $ecrl_flag = 1;
        }
    }

    if (($searchContact->search_department_yes_industry || $searchContact->search_department_no_industry) && !$edind_flag)
    {
        $sql .= ("
            LEFT JOIN industries edind
            ON ed.industry_id = edind.industry_id
            ");
        $edind_flag = 1;
    }

    if ($searchContact->search_department_size && !$edes_flag)
    {
        $sql .= ("
            LEFT JOIN employer_sizes edes
            ON ed.size_id = edes.size_id
            ");
        $edes_flag = 1;
    }
    
    if (is_array($searchContact->search_department_flags) && sizeof($searchContact->search_department_flags) > 0 && strlen($searchContact->search_department_flags_boolean))
    {
        if ($searchContact->search_department_flags_boolean == "and")
        {
            $i = 0;
            foreach ($searchContact->search_department_flags as $value)
            {
                $next = $i+1;
                if ($i == 0)
                {
                    $sql .= ("
                        LEFT JOIN department_flags_set depfs".$next."
                        ON ed.department_id = depfs".$next.".department_id
                        ");
                }

                else
                {
                    $sql .= ("
                        LEFT JOIN department_flags_set depfs".$next."
                        ON depfs".$i.".department_id = depfs".$next.".department_id
                        ");
                }
                $i++;
            }
        }

        elseif ($searchContact->search_department_flags_boolean == "or")
        {
            $sql .= ("
                LEFT JOIN department_flags_set depfs
                ON ed.department_id = depfs.department_id
                ");
        }
    }
}// end if

if ($searchContact->search_company_criteria)
{
    if (($searchContact->search_company_area || $searchContact->search_company_country) && !$eccl_flag)
    {
        $sql .= ("
            LEFT JOIN country_list eccl
            ON ec.country_id = eccl.country_id
            ");
        $eccl_flag = 1;
    }

    if ($searchContact->search_company_region && !$ecrl_flag)
    {
        $sql .= ("
            LEFT JOIN region_list ecrl
            ON ec.region_id = ecrl.region_id
            ");
        $ecrl_flag = 1;
    }

    if ($searchContact->search_company_provstate && !$ecpl_flag)
    {
        $sql .= ("
            LEFT JOIN provstate_list ecpl
            ON ec.provstate_id = ecpl.provstate_id
            ");
        $ecpl_flag = 1;
    }
    
    if (($searchContact->search_company_yes_industry || $searchContact->search_company_no_industry) && !$ecind_flag)
    {
        $sql .= ("
            LEFT JOIN industries ecind
            ON ec.industry_id = ecind.industry_id
            ");
        $ecind_flag = 1;
    }

    if ($searchContact->search_company_size && !$eces_flag)
    {
        $sql .= ("
            LEFT JOIN employer_sizes eces
            ON ec.size_id = eces.size_id
            ");
        $eces_flag = 1;
    }

    if ($searchContact->search_company_type && !$ecct_flag)
    {
        $sql .= ("
            LEFT JOIN company_type ecct
            ON ec.company_type_id = ecct.type_id
            ");
        $ecct_flag = 1;
    }

    if (is_array($searchContact->search_company_flags) && sizeof($searchContact->search_company_flags) > 0 && strlen($searchContact->search_company_flags_boolean))
    {
        if ($searchContact->search_company_flags_boolean == "and")
        {
            $i = 0;
            foreach ($searchContact->search_company_flags as $value)
            {
                $next = $i+1;
                if ($i == 0)
                {
                    $sql .= ("
                        LEFT JOIN company_flags_set comfs".$next."
                        ON ec.employer_id = comfs".$next.".employer_id
                        ");
                }

                else
                {
                    $sql .= ("
                        LEFT JOIN company_flags_set comfs".$next."
                        ON comfs".$i.".employer_id = comfs".$next.".employer_id
                        ");
                }
                $i++;
            }
        }

        elseif ($searchContact->search_company_flags_boolean == "or")
        {
            $sql .= ("
                LEFT JOIN company_flags_set comfs
                ON ec.employer_id = comfs.employer_id
                ");
        }
    }
}

// Finished dynamic joining of tables, begin working on the WHERE clause.

// make sure we don't view any contacts that have the deleted flag. 
$sql .= ("
    WHERE ce.deleted_flag = '0'
    ");
    
// Someone is viewing division/employer information via the quick menu, so we just show them default information about
// the division/comopany's contacts. 
if ($searchContact->search_division_id)
{
    $sql .= ("
        AND ce.department_id='" . addslashes($searchContact->search_division_id) . "'
        ");
}
elseif ($searchContact->search_employer_id)
{
    $sql .= ("
        AND ec.employer_id='" . addslashes($searchContact->search_employer_id) . "'
        ");
}

// Otherwise, user has come from an advanced search form, so include the search criteria they specify. 
else
{
    $department_string = "";
    if (sizeof($departments_in_group) > 0 && is_array($departments_in_group))
    {
        foreach ($departments_in_group as $department_value)
        {
            $department_string .= "'".addslashes($department_value)."',";
        }

        $department_string = substr($department_string,0,strlen($department_string)-1);
    }

	if (trim($searchContact->search_contact_first_name))
	{
		$sql .= ("
			AND c.first_name LIKE '" . addslashes($searchContact->search_contact_first_name) . "%'
			");
	}

	if (trim($searchContact->search_contact_last_name))
	{
		$sql .= ("
			AND c.last_name LIKE '" . addslashes($searchContact->search_contact_last_name) . "%'
			");
	}

	if (sizeof($searchContact->search_contact_status_status) > 0 && is_array($searchContact->search_contact_status_status)
        && sizeof($searchContact->search_contact_status_department) > 0 
        && is_array($searchContact->search_contact_status_department)
       )
	{
        foreach ($searchContact->search_contact_status_status as $scss)
        {
            $scss_string .= "'".addslashes($scss)."',";
        }

        $scss_string = substr($scss_string,0,strlen($scss_string)-1);

        foreach ($searchContact->search_contact_status_department as $scsd)
        {
            $scsd_string .= "'".addslashes($scsd)."',";
        }

        $scsd_string = substr($scsd_string,0,strlen($scsd_string)-1);

        $sql .= ("
            AND dcs.status_id IN (".$scss_string.")
            ");
		$sql .= ("
			AND dcs.department_id IN (".$scsd_string.")
			");
        $sql .= ("
            AND dcs.current_status = '1'
            ");
	}

	if (strlen(trim($searchContact->search_contact_email)) || $searchContact->search_contact_email_as == 'isblank')
	{
        // The _ character is a wild card in SQL. So, if a user wants to search with a _ in their e-mail address, 
        // the character needs to be escaped with a slash. However, if I add the slash in my object ahead of time,
        // the addslashes() function will add an extra slash to the string. This is why I'm manipulating the string
        // here, right before it is used in the query. I add slashes first, then escape any _ characters. 

        $temp_search_contact_email = addslashes($searchContact->search_contact_email);
        $temp_search_contact_email = preg_replace("/_/","\_",$temp_search_contact_email);
        
		if (!$searchContact->search_contact_email_as || $searchContact->search_contact_email_as == 'start')
		{
			$sql .= ("
				AND c.email LIKE '" . $temp_search_contact_email . "%'
				");
		}
		elseif ($searchContact->search_contact_email_as == 'containing')
		{
			$sql .= ("
				AND c.email LIKE '%" . $temp_search_contact_email . "%'
				");
		}
		elseif ($searchContact->search_contact_email_as == 'isblank')
		{
			$sql .= ("
				AND (c.email IS NULL OR c.email='')
				");
		}
	}

    if (trim(strlen($searchContact->search_contact_phone)) && $searchContact->search_contact_phone_as)
    {
        if ($searchContact->search_contact_phone_as == "start")
        {
            $sql .= ("
                AND (c.phone LIKE '".addslashes($searchContact->search_contact_phone)."%' OR c.phone = '".USE_COMPANY."' 
                     OR c.phone = '".USE_DEPARTMENT."')
                ");
        }
        else
        {
            $sql .= ("
                AND (c.phone LIKE '%".addslashes($searchContact->search_contact_phone)."%' OR c.phone = '".USE_COMPANY."' 
                     OR c.phone = '".USE_DEPARTMENT."')
                ");
        }
    }
    if (trim(strlen($searchContact->search_contact_cellphone)) && $searchContact->search_contact_cellphone_as)
    {
        if ($searchContact->search_contact_cellphone_as == "start")
        {
            $sql .= ("
                    AND (c.cellphone LIKE '".addslashes($searchContact->search_contact_cellphone)."%')
                    ");
        }
        else
        {
            $sql .= ("
                    AND (c.cellphone LIKE '%".addslashes($searchContact->search_contact_cellphone)."%')
                    ");
        }
    }
    if (trim(strlen($searchContact->search_contact_fax)) && $searchContact->search_contact_fax_as)
    {
        if ($searchContact->search_contact_fax_as == "start")
        {
            $sql .= ("
                AND (c.fax LIKE '".addslashes($searchContact->search_contact_fax)."%' OR c.fax = '".USE_COMPANY."' 
                     OR c.fax = '".USE_DEPARTMENT."')
                ");
        }
        else
        {
            $sql .= ("
                AND (c.fax LIKE '%".addslashes($searchContact->search_contact_fax)."%' OR c.fax = '".USE_COMPANY."' 
                     OR c.fax = '".USE_DEPARTMENT."')
                ");
        }
    }

	if (trim($searchContact->search_company_name))
	{
        $temp_search_company_name = addslashes($searchContact->search_company_name);

        // convert _ to \_ . This is because _ is a wild card character in SQL. 
        $temp_search_company_name = preg_replace("/_/","\_",$temp_search_company_name);

		$sql .= ("
			AND ec.company_name LIKE '" . $temp_search_company_name . "%'
			");
	}

	if (trim($searchContact->search_department_name))
	{
        $temp_search_department_name = addslashes($searchContact->search_department_name);

        // convert _ to \_ . This is because _ is a wild card character in SQL. 
        $temp_search_department_name = preg_replace("/_/","\_",$temp_search_department_name);

		$sql .= ("
			AND ed.department_name LIKE '" . $temp_search_department_name . "%'
			");
	}

	if (trim($searchContact->search_contact_department_name))
	{
        $temp_search_contact_department_name = addslashes($searchContact->search_contact_department_name);

        // convert _ to \_ . This is because _ is a wild card character in SQL. 
        $temp_search_contact_department_name = preg_replace("/_/","\_",$temp_search_contact_department_name);
        
		$sql .= ("
			AND ce.department_name LIKE '" . $temp_search_contact_department_name . "%'
			");
	}

    if (trim($searchContact->search_contact_country))
    {
        $sql .= ("
                AND (
                    c.country_id='" . addslashes($searchContact->search_contact_country) . "'
                    OR c.country_id = '0' OR c.country_id IS NULL
                    )
                ");
    }

    if (trim($searchContact->search_contact_provstate))
    {
        $sql .= ("
                AND (
                    c.provstate_id='" . addslashes($searchContact->search_contact_provstate) . "'
                    OR c.provstate_id = '0' OR c.country_id IS NULL
                    )
                ");
    }

    if (trim($searchContact->search_contact_region))
    {
        $sql .= ("
                AND (
                    c.region_id ='".addslashes($searchContact->search_contact_region)."'
                    OR c.region_id = '0' OR c.region_id IS NULL
                    )
                ");
    }

    if (trim(strlen($searchContact->search_contact_postal_code)))
    {
        $sql .= ("
            AND (c.postal_code LIKE '".addslashes($searchContact->search_contact_postal_code)."%' 
            OR c.postal_code = '' OR c.postal_code IS NULL)
            ");
    }

    if (trim(strlen($searchContact->search_contact_city)))
    {
        $sql .= ("
            AND (c.city LIKE '".addslashes($searchContact->search_contact_city)."%' OR c.city = '' OR c.city IS NULL)
            ");
    }

    if (trim($searchContact->search_contact_action))
    {
        $sql .= ("
             AND cas.action_id='" . addslashes($searchContact->search_contact_action) . "'
             ");
    }

    // From the advanced search form, we get a contact_id from a staff member.
    if (trim($searchContact->search_contact_action_by))
    {
        // We grab their the name and netlink_id
        $employee_info_sql = ("
            SELECT c.first_name, c.last_name, ci.netlink_id
            FROM contact c 
            INNER JOIN contact_internal ci
            ON c.contact_id = ci.contact_id
            WHERE c.contact_id = '".$searchContact->search_contact_action_by."'
            ");
        $employee_info_result = $GLOBALS['dbh']->Execute($employee_info_sql);
        if ($employee_info_result->RecordCount())
        {
            $employee_info_row = $employee_info_result->FetchRow();
            
            // And we use that name and netlink_id to get ALL contact ids that is this same person. We're assuming that the combination of 
            // a first_name, last_name, and netlink_id is unique among users. 
            $employee_sql = ("
                SELECT ci.contact_id, ci.netlink_id, ci.department_id
                FROM contact c 
                INNER JOIN contact_internal ci
                ON c.contact_id = ci.contact_id
                WHERE c.first_name = '".addslashes($employee_info_row["first_name"])."'
                AND c.last_name = '".addslashes($employee_info_row["last_name"])."'
                AND ci.netlink_id = '".addslashes($employee_info_row["netlink_id"])."'
                ");
            $employee_result = $GLOBALS['dbh']->Execute($employee_sql);

            if ($employee_result->RecordCount())
            {
                $arrContactID = array();
                
                $index = 0;
                while($employee_row = $employee_result->FetchRow())
                {
                    // If we find that the individual is inactive within our own department, then we only want the contact id for that
                    // inactive staff member. Staff is inactive if their netlink_id == CONTACT_INTERNAL_INACTIVE
                    if ($auth->department == $employee_row['department_id'] && $employee_row['netlink_id'] == CONTACT_INTERNAL_INACTIVE)
                    {
                        $arrContactID = NULL;
                        $arrContactID[0] = $employee_row['contact_id'];
                        break;
                    }

                    // Otherwise, get all the contact ids from the employee number where the contact is not inactive. 
                    elseif($employee_row['netlink_id'] != CONTACT_INTERNAL_INACTIVE)
                    {
                        $arrContactID[] = $employee_row['contact_id'];
                    }
                }

                if (sizeof($arrContactID))
                {
                    $contact_id_in_str = "'".implode($arrContactID,"','")."'";
                    $sql .= ("
                        AND cas.action_by IN (".$contact_id_in_str.")
                        ");
                }
            }
        }
    }

    if (trim($searchContact->search_contact_action_on) && trim($searchContact->search_contact_action_as))
    {
		if ($searchContact->search_contact_action_as == 'on')
		{
			$sql .= ("
				AND cas.action_on='" . addslashes($searchContact->search_contact_action_on) . "'
				");
		}
		elseif ($searchContact->search_contact_action_as == 'after')
		{
			$sql .= ("
				AND cas.action_on >= '" . addslashes($searchContact->search_contact_action_on) . "'
				");
		}
		elseif ($searchContact->search_contact_action_as == 'before')
		{
			$sql .= ("
				AND cas.action_on <= '" . addslashes($searchContact->search_contact_action_on) . "'
				");
		}
    }

    if (trim($searchContact->search_contact_action_between1) && trim($searchContact->search_contact_action_between2))
    {
        $sql .= ("
            AND cas.action_on >= '" . addslashes($searchContact->search_contact_action_between1) . "'
            AND cas.action_on <='" . addslashes($searchContact->search_contact_action_between2) . "'
            ");
    }

    elseif (trim($searchContact->search_contact_action_between1) && !trim($searchContact->search_contact_action_between2))
    {
        $sql .= ("
            AND cas.action_on >= '" . addslashes($searchContact->search_contact_action_between1) . "'
            ");
    }

    elseif (!trim($searchContact->search_contact_action_between1) && trim($searchContact->search_contact_action_between2))
    {
        $sql .= ("
            AND cas.action_on <='" . addslashes($searchContact->search_contact_action_between2) . "'
            ");
    }

    if (trim($searchContact->search_contact_action_bf_on) && trim($searchContact->search_contact_action_bf_as))
    {
		if ($searchContact->search_contact_action_bf_as == 'on')
		{
			$sql .= ("
				AND cas.bring_forward_date ='" . addslashes($searchContact->search_contact_action_bf_on) . "'
				");
		}
		elseif ($searchContact->search_contact_action_bf_as == 'after')
		{
			$sql .= ("
				AND cas.bring_forward_date >= '" . addslashes($searchContact->search_contact_action_bf_on) . "'
				");
		}
		elseif ($searchContact->search_contact_action_bf_as == 'before')
		{
			$sql .= ("
				AND cas.bring_forward_date <= '" . addslashes($searchContact->search_contact_action_bf_on) . "'
				");
		}
    }

    if (trim($searchContact->search_contact_action_bf_between1) && trim($searchContact->search_contact_action_bf_between2))
    {
        $sql .= ("
            AND cas.bring_forward_date >= '" . addslashes($searchContact->search_contact_action_bf_between1) . "'
            AND cas.bring_forward_date <='" . addslashes($searchContact->search_contact_action_bf_between2) . "'
            ");
    }

    elseif (trim($searchContact->search_contact_action_bf_between1) && !trim($searchContact->search_contact_action_bf_between2))
    {
        $sql .= ("
            AND cas.bring_forward_date >= '" . addslashes($searchContact->search_contact_action_bf_between1) . "'
            ");
    }

    elseif (!trim($searchContact->search_contact_action_bf_between1) && trim($searchContact->search_contact_action_bf_between2))
    {
        $sql .= ("
            AND cas.bring_forward_date <='" . addslashes($searchContact->search_contact_action_bf_between2) . "'
            ");
    }

    // Anytime we're searching on anything based on actions, we don't want to include automated actions as mentioned below. When
    // $cas_flag evaluates to true, that means we're searching on something based on actions. 
    if ($cas_flag)
    {
   //     $sql .= ("
     //        AND cas.action_id NOT IN ('".GENERIC_NOTE."','".CONDUCTED_WORKSITE_VISIT."','".INT_CONDUCTED."',
       //      '".JOB_DESC_RECEIVED."','".FINAL_PLACEMENT."')
         //    ");
    }

    if (sizeof($searchContact->search_contact_flags) > 0 && is_array($searchContact->search_contact_flags) 
        && strlen(trim($searchContact->search_contact_flags_boolean))
       )
    {
        if ($searchContact->search_contact_flags_boolean == 'and')
        {
            $i=0;
            foreach($searchContact->search_contact_flags as $flag_value)
            {
                $next = $i+1;
                $sql .= ("
                    AND cfs".$next.".flag_id = '".addslashes($flag_value)."'
                    ");
                $i++;
            }
        }

        elseif ($searchContact->search_contact_flags_boolean == 'or')
        {
            $cfs_string = "";
            foreach($searchContact->search_contact_flags as $flag_value)
            {
               $cfs_string .= "'".addslashes($flag_value)."',";
            }

            $cfs_string = substr($cfs_string,0,strlen($cfs_string)-1);

            $sql .= ("
                AND cfs.flag_id IN (".$cfs_string.")
                ");
        }
    }

	// If they came here from the company/division criteria screen, we need to add those
	// criteria to the query as well.
	if ($searchContact->search_department_criteria)
	{
		if (trim($searchContact->search_department_website))
		{
            $temp_search_department_website = addslashes($searchContact->search_department_website);

            // convert _ to \_ . This is because _ is a wild card character in SQL. 
            $temp_search_department_website = preg_replace("/_/","\_",$temp_search_department_website);

			if ($searchContact->search_department_website_as == 'start' || !$searchContact->search_department_website_as)
			{
				$sql .= ("
					AND ed.department_website LIKE '" . $temp_search_department_website . "%'
					");
			}
			elseif ($searchContact->search_department_website_as == 'containing')
			{
				$sql .= ("
					AND ed.department_website LIKE '%" . $temp_search_department_website . "%'
					");
			}
		}

		if (trim($searchContact->search_department_email))
		{
            $temp_search_department_email = addslashes($searchContact->search_department_email);

            // convert _ to \_ . This is because _ is a wild card character in SQL. 
            $temp_search_department_email = preg_replace("/_/","\_",$temp_search_department_email);

			if ($searchContact->search_department_email_as == 'start' || !$searchContact->search_department_email_as)
			{
				$sql .= ("
					AND ed.email LIKE '" . $temp_search_department_email . "%'
					");
			}
			elseif ($searchContact->search_department_email_as == 'containing')
			{
				$sql .= ("
					AND ed.email LIKE '%" . $temp_search_department_email . "%'
					");
			}
		}

        if (trim(strlen($searchContact->search_department_phone)) && $searchContact->search_department_phone_as)
        {
            if ($searchContact->search_department_phone_as == "start")
            {
                $sql .= ("
                    AND (ed.phone LIKE '".addslashes($searchContact->search_department_phone)."%' OR ed.phone = '".USE_COMPANY."')
                    ");
            }
            else
            {
                $sql .= ("
                    AND (c.phone LIKE '%".addslashes($searchContact->search_contact_phone)."%' OR c.phone = '".USE_COMPANY."')
                    ");
            }
        }

        if (trim(strlen($searchContact->search_department_fax)) && $searchContact->search_department_fax_as)
        {
            if ($searchContact->search_department_fax_as == "start")
            {
                $sql .= ("
                    AND (ed.fax LIKE '".addslashes($searchContact->search_department_fax)."%' OR ed.fax = '".USE_COMPANY."')
                    ");
            }
            else
            {
                $sql .= ("
                    AND (ed.fax LIKE '%".addslashes($searchContact->search_department_fax)."%' OR ed.fax = '".USE_COMPANY."')
                    ");
            }
        }

		if ($searchContact->search_department_yes_industry)
		{
            $department_yes_string = implode("','",$searchContact->search_department_yes_industry);
            $department_yes_string = "'".$department_yes_string."'";

			$sql .= ("
				AND ed.industry_id IN (".$department_yes_string.")
				");
		}

		if ($searchContact->search_department_no_industry)
		{
            $department_no_string = implode("','",$searchContact->search_department_no_industry);
            $department_no_string = "'".$department_no_string."'";

			$sql .= ("
				AND ed.industry_id NOT IN (".$department_no_string.")
				");
		}

		if (is_array($searchContact->search_department_size) && sizeof($searchContact->search_department_size))
		{
			$sql .= ("
				AND (
				");

			for ($i = 0; $i < (sizeof($searchContact->search_department_size) - 1); $i++)
			{
				$sql .= ("
					ed.size_id='" . addslashes($searchContact->search_department_size[$i]) . "' OR
					");
			}

			$sql .= ("
				ed.size_id='" . addslashes($searchContact->search_department_size[$i]) . "'
				");
			$sql .= ("
				)
				");
		}
        
        if (trim($searchContact->search_department_country))
        {
            $sql .= ("
                    AND (
                        ed.country_id='" . addslashes($searchContact->search_department_country) . "'
                        OR ed.country_id = '0' OR ed.country_id IS NULL
                        )
                    ");
        }

        if (trim($searchContact->search_department_provstate))
        {
            $sql .= ("
                    AND (
                        ed.provstate_id='" . addslashes($searchContact->search_department_provstate) . "'
                        OR ed.provstate_id = '0' OR ed.country_id IS NULL
                        )
                    ");
        }

        if (trim($searchContact->search_department_region))
        {
            $sql .= ("
                    AND (
                        ed.region_id ='".addslashes($searchContact->search_department_region)."'
                        OR ed.region_id = '0' OR ed.region_id IS NULL
                        )
                    ");
        }

        if (trim(strlen($searchContact->search_department_postal_code)))
        {
            $sql .= ("
                AND (ed.postal_code LIKE '".addslashes($searchContact->search_department_postal_code)."%' 
                OR ed.postal_code = '' OR ed.postal_code IS NULL)
                ");
        }

        if (trim(strlen($searchContact->search_department_city)))
        {
            $sql .= ("
                AND (ed.city LIKE '".addslashes($searchContact->search_department_city)."%' OR ed.city = '' OR ed.city IS NULL)
                ");
        }

        if (sizeof($searchContact->search_department_flags) > 0 && is_array($searchContact->search_department_flags) 
            && strlen(trim($searchContact->search_department_flags_boolean))
           )
        {
            if ($searchContact->search_department_flags_boolean == 'and')
            {
                $i=0;
                foreach($searchContact->search_department_flags as $flag_value)
                {
                    $next = $i+1;
                    $sql .= ("
                        AND depfs".$next.".flag_id = '".addslashes($flag_value)."'
                        ");
                    $i++;
                }
            }

            elseif ($searchContact->search_department_flags_boolean == 'or')
            {
                $depfs_string = "";
                foreach($searchContact->search_department_flags as $flag_value)
                {
                   $depfs_string .= "'".addslashes($flag_value)."',";
                }

                $depfs_string = substr($depfs_string,0,strlen($depfs_string)-1);

                $sql .= ("
                    AND depfs.flag_id IN (".$depfs_string.")
                    ");
            }
        }
	}

	if ($searchContact->search_company_criteria)
	{
		if (trim($searchContact->search_company_website))
		{
            $temp_search_company_website = addslashes($searchContact->search_company_website);

            // convert _ to \_ . This is because _ is a wild card character in SQL. 
            $temp_search_company_website = preg_replace("/_/","\_",$temp_search_company_website);

			if ($searchContact->search_company_website_as == 'start' || !$searchContact->search_company_website_as)
			{
				$sql .= ("
					AND ec.website LIKE '" . $temp_search_company_website . "%'
					");
			}
			elseif ($searchContact->search_company_website_as == 'containing')
			{
				$sql .= ("
					AND ec.website LIKE '%" . $temp_search_company_website . "%'
					");
			}
		} 

		if (trim($searchContact->search_company_email))
		{
            $temp_search_company_email = addslashes($searchContact->search_company_email);

            // convert _ to \_ . This is because _ is a wild card character in SQL. 
            $temp_search_company_email = preg_replace("/_/","\_",$temp_search_company_email);

		 	if ($searchContact->search_company_email_as == 'start' || !$searchContact->search_company_email_as)
			{
				$sql .= ("
					AND ec.email LIKE '" . $temp_search_company_email . "%'
					");
			}
			elseif ($searchContact->search_company_email_as == 'containing')
			{
				$sql .= ("
					AND ec.email LIKE '%" . $temp_search_company_email . "%'
					");
			}
		}

		if (trim($searchContact->search_company_phone))
		{
			if ($searchContact->search_company_phone_as == 'start' || !$searchContact->search_company_phone_as)
			{
				$sql .= ("
					AND ec.phone LIKE '" . addslashes($searchContact->search_company_phone) . "%'
					");
			}
			elseif ($searchContact->search_company_phone_as == 'containing')
			{
				$sql .= ("
					AND ec.phone LIKE '%" . addslashes($searchContact->search_company_phone) . "%'
					");
			}
		}

		if (trim($searchContact->search_company_fax))
		{
			if ($searchContact->search_company_fax_as == 'start' || !$searchContact->search_company_fax_as)
			{
				$sql .= ("
					AND ec.fax LIKE '" . addslashes($searchContact->search_company_fax) . "%'
					");
			}
			elseif ($searchContact->search_company_fax_as == 'containing')
			{
				$sql .= ("
					AND ec.fax LIKE '%" . addslashes($searchContact->search_company_fax) . "%'
					");
			}
		}

		if ($searchContact->search_company_yes_industry)
		{
            $company_yes_string = implode("','",$searchContact->search_company_yes_industry);
            $company_yes_string = "'".$company_yes_string."'";

			$sql .= ("
				AND ec.industry_id IN (".$company_yes_string.")
				");
		}

		if ($searchContact->search_company_no_industry)
		{
            $company_no_string = implode("','",$searchContact->search_company_no_industry);
            $company_no_string = "'".$company_no_string."'";

			$sql .= ("
				AND ec.industry_id NOT IN (".$company_no_string.")
				");
		}

		if ($searchContact->search_company_size)
		{
			$sql .= ("
				AND (
				");

			for ($i = 0; $i < (sizeof($searchContact->search_company_size) - 1); $i++)
			{
				$sql .= ("
					ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "' OR
					");
			}
			
			$sql .= ("
				ec.size_id='" . addslashes($searchContact->search_company_size[$i]) . "'
				");
			$sql .= ("
				)
				");
		}
						
        if ($searchContact->search_company_type)
        {
            $sql .= ("
                AND (
                ");

            for ($i = 0; $i < (sizeof($searchContact->search_company_type) - 1); $i++)
            {
                $sql .= ("
                    ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "' OR
                    ");
            }
            
            $sql .= ("
                ec.company_type_id='" . addslashes($searchContact->search_company_type[$i]) . "'
                ");

            $sql .= ("
                )
                ");
        }

		if (trim($searchContact->search_company_area))
		{        
			$sql .= ("
				AND eccl.area_id='" . addslashes($searchContact->search_company_area) . "'
				");     
		}               
					
		if (trim($searchContact->search_company_country))
		{       
			$sql .= ("
				AND ec.country_id='" . addslashes($searchContact->search_company_country) . "'
				");
		}               
					
		if (trim($searchContact->search_company_provstate))
		{
			$sql .= ("
				AND ec.provstate_id='" . addslashes($searchContact->search_company_provstate) . "'
				");     
		}                               
						
		if (trim($searchContact->search_company_region))
		{
			$sql .= ("
				AND ec.region_id='" . addslashes($searchContact->search_company_region) . "'
				");     
		}               
					
		if (trim($searchContact->search_company_city))
		{       
			$sql .= ("
				AND ec.city LIKE '" . addslashes($searchContact->search_company_city) . "%'
				");
		}
		
		if (trim($searchContact->search_company_postal_code))
		{
			$sql .= ("
				AND ec.postal_code LIKE '" . addslashes($searchContact->search_company_postal_code) . "%'
				");
		}
        
        if (sizeof($searchContact->search_company_flags) > 0 && is_array($searchContact->search_company_flags) 
            && strlen(trim($searchContact->search_company_flags_boolean))
           )
        {
            if ($searchContact->search_company_flags_boolean == 'and')
            {
                $i=0;
                foreach($searchContact->search_company_flags as $flag_value)
                {
                    $next = $i+1;
                    $sql .= ("
                        AND comfs".$next.".flag_id = '".addslashes($flag_value)."'
                        ");
                    $i++;
                }
            }

            elseif ($searchContact->search_company_flags_boolean == 'or')
            {
                $comfs_string = "";
                foreach($searchContact->search_contact_flags as $flag_value)
                {
                   $comfs_string .= "'".addslashes($flag_value)."',";
                }

                $comfs_string = substr($comfs_string,0,strlen($comfs_string)-1);

                $sql .= ("
                    AND comfs.flag_id IN (".$comfs_string.")
                    ");
            }
        }
	}
}
// Ah... almost done... now finish this query off by applying the order by clauses.

$sql .= ("
    ORDER BY
    ");

if (sizeof($searchContact->search_order) <= 0 || !is_array($searchContact->search_order))
{
    $sql .= ("
        c.last_name, c.first_name
        ");
}

elseif (sizeof($searchContact->search_order) > 0 && is_array($searchContact->search_order))
{
    $order_string = "";
    foreach ($searchContact->search_order as $order_value)
    {
        $order_string .= addslashes($display_array[$order_value]["order"]).", ";
    }
    
    $order_string = trim($order_string);
    $order_string = substr($order_string,0,strlen($order_string)-1);

    $sql .= $order_string;
}

set_time_limit(60);
$results = $GLOBALS['dbh']->Execute($sql);
$precount = $results->RecordCount();
$count = 0;

if ($precount > 0)
{
    // This block checks to see if any post query filtering is required
    if (!strlen(trim($searchContact->search_division_id)) || !strlen(trim($searchContact->search_employer_id)))
    {
        // This block of code here prepares to filter out rows that we don't want from the main query. The reason
        // we do a post-query filter is because a resulting COLUMN selected using a dynamic IF statement cannot be given
        // restrictions in the WHERE clause of a SQL query. For example, if I wanted to know if a contact was in Canada,
        // and my IF statement looks in division, and company for this info, and returns this answer AS c_country_name,
        // this alias cannot be referenced in the WHERE clause. WHERE c_country_name = 'Canada' is not a valid SQL statement.
        // and that is why we have a post-query filter right here.

        // The filter command stores individual expressions for a large IF statement. This algorithm's speed depends on 
        // logically short-circuiting the IF statement. The idea is, if one of these expressions are satisfied then the
        // row it is evaluating will be deleted. By dynamically creating this IF statement, we also limit the effects
        // of a worse-case scenario, which means the IF statement has to check all its expressions. 
        $filter_command = array();

        if (strlen(trim($searchContact->search_contact_area)))
        {
            $contact_area = $searchContact->search_contact_area;
            $filter_command[] = ("
                \$row[\"c_area_id\"] != \$contact_area
                ");
        }

        if (strlen(trim($searchContact->search_contact_country)))
        {
            $contact_country_name = getCountryName(addslashes($searchContact->search_contact_country));
            if (strlen($contact_country_name))
            {
                $filter_command[] = ("
                    \$row[\"c_country_name\"] != \$contact_country_name
                    ");
            }
        }
        
        if (strlen(trim($searchContact->search_contact_provstate)))
        {
            $contact_provstate_name = getProvstateName(addslashes($searchContact->search_contact_provstate));
            if (strlen($contact_provstate_name))
            {
                $filter_command[] = ("
                    \$row[\"c_provstate_name\"] != \$contact_provstate_name
                    ");
            }
        }

        if (strlen(trim($searchContact->search_contact_region)))
        {
            $contact_region_name = getRegionName(addslashes($searchContact->search_contact_region));
            if (strlen($contact_region_name))
            {
                $filter_command[] = ("
                    \$row[\"c_region_name\"] != \$contact_region_name
                    ");
            }
        }
        
        // Department specific filters

        if (strlen(trim($searchContact->search_department_area)))
        {
            $department_area = $searchContact->search_department_area;
            $filter_command[] = ("
                \$row[\"ed_area_id\"] != \$department_area
                ");
        }

        if (strlen(trim($searchContact->search_department_country)))
        {
            $department_country_name = getCountryName(addslashes($searchContact->search_department_country));
            if (strlen($department_country_name))
            {
                $filter_command[] = ("
                    \$row[\"ed_country_name\"] != \$department_country_name
                    ");
            }
        }
        
        if (strlen(trim($searchContact->search_department_provstate)))
        {
            $department_provstate_name = getProvstateName(addslashes($searchContact->search_department_provstate));
            if (strlen($department_provstate_name))
            {
                $filter_command[] = ("
                    \$row[\"ed_provstate_name\"] != \$department_provstate_name
                    ");
            }
        }

        if (strlen(trim($searchContact->search_department_region)))
        {
            $department_region_name = getRegionName(addslashes($searchContact->search_department_region));
            if (strlen($department_region_name))
            {
                $filter_command[] = ("
                    \$row[\"ed_region_name\"] != \$department_region_name
                    ");
            }
        }
        
        // The statements here tend to be the most expensive to run, so we leave them till last to execute in the IF
        // statement. If the IF statement short-circuits before these criteria, we save some time by not needing to
        // run these expensive regular expressions.

        if (strlen(trim($searchContact->search_contact_city)))
        {
            $contact_city = $searchContact->search_contact_city;
            // This represents !preg_match("/^".$contact_city."/",$row["c_city"])
            // the regexp looks for a string that starts with the $contact_city string.
            $filter_command[] = ("
                !preg_match(\"/^\".\$contact_city.\"/\",\$row[\"c_city\"])
                ");
        }
        
        if (strlen(trim($searchContact->search_department_city)))
        {
            $department_city = $searchContact->search_department_city;
            $filter_command[] = ("
                !preg_match(\"/^\".\$department_city.\"/\",\$row[\"ed_city\"])
                ");
        }

        if (strlen(trim($searchContact->search_contact_postal_code)))
        {
            $contact_postal_code = $searchContact->search_contact_postal_code;
            $filter_command[] = ("
                !preg_match(\"/^\".\$contact_postal_code.\"/\",\$row[\"c_postal_code\"])
                ");
        }

        if (strlen(trim($searchContact->search_department_postal_code)))
        {
            $department_postal_code = $searchContact->search_department_postal_code;
            $filter_command[] = ("
                !preg_match(\"/^\".\$department_postal_code.\"/\",\$row[\"ed_postal_code\"])
                ");
        }

        if (trim($searchContact->search_contact_phone_as) && strlen(trim($searchContact->search_contact_phone)))
        {
            if ($searchContact->search_contact_phone_as == "start")
            {
                $contact_phone = addslashes($searchContact->search_contact_phone);
                // !preg_match("/^".$contact_phone."/",$row["c_phone"]); if phone doesn't start with specified digits, reject.
                $filter_command[] = ("
                    !preg_match(\"/^\".\$contact_phone.\"/\",\$row[\"c_phone\"])
                    ");
            }
            
            elseif ($searchContact->search_contact_phone_as == "containing")
            {
                $contact_phone = addslashes($searchContact->search_contact_phone);
                $filter_command[] = ("
                    !preg_match(\"/\".\$contact_phone.\"/\",\$row[\"c_phone\"])
                    ");
            }
        }
        
       if (trim($searchContact->search_contact_cellphone_as) && strlen(trim($searchContact->search_contact_cellphone)))
        {
            if ($searchContact->search_contact_cellphone_as == "start")
            {
                $contact_cellphone = addslashes($searchContact->search_contact_cellphone);
                // !preg_match("/^".$contact_phone."/",$row["c_phone"]); if phone doesn't start with specified digits, reject.
                $filter_command[] = ("
                        !preg_match(\"/^\".\$contact_cellphone.\"/\",\$row[\"c_cellphone\"])
                        ");
            }

            elseif ($searchContact->search_contact_cellphone_as == "containing")
            {
                $contact_cellphone = addslashes($searchContact->search_contact_cellphone);
                $filter_command[] = ("
                        !preg_match(\"/\".\$contact_cellphone.\"/\",\$row[\"c_cellphone\"])
                        ");
            }
        } 
        if (trim($searchContact->search_department_phone_as) && strlen(trim($searchContact->search_department_phone)))
        {
            if ($searchContact->search_department_phone_as == "start")
            {
                $department_phone = addslashes($searchContact->search_department_phone);
                $filter_command[] = ("
                    !preg_match(\"/^\".\$department_phone.\"/\",\$row[\"ed_phone\"])
                    ");
            }
            
            elseif ($searchContact->search_department_phone_as == "containing")
            {
                $department_phone = addslashes($searchContact->search_department_phone);
                $filter_command[] = ("
                    !preg_match(\"/\".\$department_phone.\"/\",\$row[\"ed_phone\"])
                    ");
            }
        }

        if (trim($searchContact->search_contact_fax_as) && strlen(trim($searchContact->search_contact_fax)))
        {
            if ($searchContact->search_contact_fax_as == "start")
            {
                $contact_fax = addslashes($searchContact->search_contact_fax);
                $filter_command[] = ("
                    !preg_match(\"/^\".\$contact_fax.\"/\",\$row[\"c_fax\"])
                    ");
            }
            
            elseif ($searchContact->search_contact_fax_as == "containing")
            {
                $contact_fax = addslashes($searchContact->search_contact_fax);
                $filter_command[] = ("
                    !preg_match(\"/\".\$contact_fax.\"/\",\$row[\"c_fax\"])
                    ");
            }
        }

        if (trim($searchContact->search_department_fax_as) && strlen(trim($searchContact->search_department_fax)))
        {
            if ($searchContact->search_department_fax_as == "start")
            {
                $department_fax = addslashes($searchContact->search_department_fax);
                $filter_command[] = ("
                    !preg_match(\"/^\".\$department_fax.\"/\",\$row[\"ed_fax\"])
                    ");
            }
            
            elseif ($searchContact->search_department_fax_as == "containing")
            {
                $department_fax = addslashes($searchContact->search_department_fax);
                $filter_command[] = ("
                    !preg_match(\"/\".\$department_fax.\"/\",\$row[\"ed_fax\"])
                    ");
            }
        }

        if (sizeof($filter_command) > 0 && is_array($filter_command))
        {
            $command = implode(" || ",$filter_command);
            $command = "if(".$command.") { \$drop_flag = 1; }";
        }

    }//end if

    // Get the query's associative array's keys.

    $arrKeys = array();
    $row = $results->FetchRow();
    $i = 0;
    foreach ($row as $key => $value)
    {
        $arrKeys[$i] = $key;
        $i++;
    }
    $results->Move(0);

    // Copy everything from our query into the $arrRow array.
    $arrRow = array();
    $i = 0;

    if ($nflags)
    {
        //filter stuff out
    }

    $drop_flag = 0;

    while ($row = $results->FetchRow())
    {
        // Begin using dynamic IF statement to drop rows. If the row passes all conditions, then it is copied to the
        // final array that stores our final results. 

        //if arrRow's contact_id = NULL, skip it
        // If we have to search for 'NO' contact flags, then we must do a post query filtering of the results. 
        if (sizeof($searchContact->search_contact_nflags) > 0 && is_array($searchContact->search_contact_nflags) 
            && strlen(trim($searchContact->search_contact_flags_boolean))
           )
        {
            if ($searchContact->search_contact_flags_boolean == "and")
            {
                $nflag_sql = ("
                    SELECT flag_id
                    FROM contact_flags_set
                    WHERE contact_id = '".$row['contact_id']."'
                    ");
                $nflag_result = $GLOBALS['dbh']->Execute($nflag_sql);

                if ($nflag_result->RecordCount())
                {
                    $temp_flag_array = array();
                    
                    while ($nflag_row  = $nflag_result->FetchRow())
                    {
                        $temp_flag_array[] = $nflag_row['flag_id'];
                    }

                    foreach ($searchContact->search_contact_nflags as $nflag_value)
                    {
                        if (in_array($nflag_value,$temp_flag_array))
                        {
                            $drop_flag = 1;
                            break;
                        }
                    }
                }
            }
            elseif ($searchContact->search_contact_flags_boolean == "or")
            {
                $nflag_sql = ("
                    SELECT flag_id
                    FROM contact_flags_set
                    WHERE contact_id = '".$row['contact_id']."'
                    ");
                $nflag_result = $GLOBALS['dbh']->Execute($nflag_sql);

                if ($nflag_result->RecordCount())
                {
                    $temp_flag_array = array();
                    
                    while ($nflag_row  = $nflag_result->FetchRow())
                    {
                        $temp_flag_array[] = $nflag_row['flag_id'];
                    }

                    $drop_flag = 1;
                    foreach ($searchContact->search_contact_nflags as $nflag_value)
                    {
                        if (!in_array($nflag_value,$temp_flag_array))
                        {
                            $drop_flag = 0;
                            break;
                        }
                    }
                }
            }
        }
        if (!$drop_flag &&strlen($command))
        {
            eval($command);
        }
        if(!$drop_flag)
        {
            for($j=0;$j<sizeof($arrKeys);$j++)
            {
                $arrRow[$i][$arrKeys[$j]] = $row[$arrKeys[$j]];
            }
            $i++;
        }
        $drop_flag = 0;
    }

    /*
    // If we didn't need to filter anything, we just copy everything over to array. 
    else
    {
        while ($row = $results->FetchRow())
        {
            for($j=0;$j<sizeof($arrKeys);$j++)
            {
                $arrRow[$i][$arrKeys[$j]] = $row[$arrKeys[$j]];
            }
            $i++;
        }
    }
    */

    $results->Close();

    // For debugging purposes.

    /*
       for($i=0;$i<sizeof($arrRow);$i++)
       {
           print $i.". ";

           for($j=0;$j<sizeof($arrKeys);$j++)
           {
               print $arrRow[$i][$arrKeys[$j]]."\t";
           }

           print "<br />";
       }
     */

} // end if 

$count = sizeof($arrRow);

// Okay, this part isn't the most elegent solution... but here goes. If a user wants to display all contact flags associated with a contact, then we have to
// bring back a list of contacts for each contact_id. This is probably easy to do if we had nested queries... but we don't. 

$need_contact_flags = 0;

for ($i=1;$i<=sizeof($searchContact->search_display);$i++)
{
    if ($display_array[$searchContact->search_display[$i]]["name"] == "Contact Flags")
    {
        $need_contact_flags = 1;
        break;
    }
}

if ($count && $need_contact_flags)
{
    for ($i = 0; $i < $count; $i++)
    {
        $flag_contact_id = $arrRow[$i]["contact_id"];

        $flag_sql = ("
            SELECT DISTINCT cf.flag_name
            FROM department_contact_flags dcf
            INNER JOIN contact_flags_set cfs
            ON dcf.flag_id = cfs.flag_id AND dcf.department_id = '".$auth->department."'
            INNER JOIN contact_flags cf
            ON cfs.flag_id = cf.flag_id
            WHERE cfs.contact_id = '".$flag_contact_id."'
            ORDER BY cf.flag_name
            ");
        $flag_result = $GLOBALS['dbh']->Execute($flag_sql);
        
        if ($flag_result->RecordCount())
        {
            $flag_str = "";
            while ($flag_row = $flag_result->FetchRow())
            {
                $flag_str .= $flag_row["flag_name"].", ";
            }
            $flag_str = trim($flag_str);
            $flag_str = substr($flag_str, 0, (strlen($flag_str) - 1));
            $arrRow[$i]["contact_flags"] = $flag_str;
        }
    }
}

// Begin outputting display

echo("<table>");
echo("<tr>");
echo("<td>");
// A form that allows the user to jump directly back to the advanced search. 
echo("<form name='advancedsearch' action='".$PHP_SELF."&amp;select=view_contact' method='post'>\n");
echo("<input type='hidden' name='continue' value='Go to Advanced Search' />\n");
echo("<input type='hidden' name='level1' value='contact' />\n");
echo("<input type='submit' name='anonymous' value='New Search' />");
echo("</form>");
echo("&nbsp;");
echo("</td>");
echo("<td>");
echo("<form name='edit_search_criteria_form' action='".$PHP_SELF."&amp;select=view_contact' method='post'>\n");
echo("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />\n");
echo("<input type='hidden' name='continue' value='Go to Advanced Search' />\n");
echo("<input type='hidden' name='level1' value='contact' />\n");
echo("<input type='submit' name='btnSearch' value='Edit Search Criteria' />");
echo("</form>");
echo("&nbsp;");
echo("</td>");
// :TODO Temporarily disable saved searches for searches that contain company/division criteria. 
if(!$search_saved && (!$searchContact->search_company_criteria && !$searchContact->search_department_criteria))
{
    echo("<td>");
    echo("<form method='post' name='myform' action='$PHP_SELF'>");
    echo("<input type='hidden' name='select' value='view_contact' />");
    echo("<input type='hidden' name='level1' value='contact' />");
    echo("<input type='hidden' name='continue' value='save_contact_search' />");
    echo("<input type='submit' name='btnSearch' value='Save Search' />");
    echo("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />");
    echo("<input type='hidden' name='search_comp_or_div' value='contact' />");
    echo("</form>");
    echo("&nbsp;");
    echo("</td>");
}

echo("</tr>");
echo("</table>");
if ($count == 0)
{
    notify("There are currently no contacts in the database that match the criteria you have searched on.");
}

else
{
    // These variables will help reduce the problem of too many contacts on a screen

    if ($per_page_max == "") { $per_page_max = 50; }
    if ($per_page_max < 5) { $per_page_max = 5; }
    if ($start_row == '') { $start_row = 0; }

    $pages = ceil($count / $per_page_max);

    $first = $start_row;
    //$first = $start_row + 1;
    $end = $start_row + $per_page_max - 1;
    if ($end > $count) { $end = $count - 1; }

    $contact_id_list = "";

    // contact_id_list is a comma delimited list of contact_id's returned. This list is used for other pieces of code 
    // that relies on the results of this advanced search. 
    for($i=0;$i<$count;$i++)
    {
        $contact_id_list .= $arrRow[$i]["contact_id"].",";
    }

    $contact_id_list = substr($contact_id_list, 0, (strlen($contact_id_list) - 1));

    if ($count <= $per_page_max)
    {
        echo("<h4 align='left'>".$count . " contact" . (($count != 1) ? "s" : "") . ":</h4>");
    }
    else
    {
        echo("<h4 align='left'>Results of search:</h4>");
    }

    echo("<form method='post' action='" . $PHP_SELF . "' name='main_form'>\n");
    echo("<input type='hidden' name='select' value='view_contact' />\n");
    echo("<input type='hidden' name='level1' value='contact' />\n");
    echo("<input type='hidden' name='continue' value='view_specific_contact' />\n");
    echo("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />\n");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />\n");
	echo("<input type='hidden' name='contact_id_string' value=\"" . packObject($contact_id_list) . "\" />");
    // the export hidden variable is used to determine if we want to export this data as a tab delimited file,
    // there's javascript at the very bottom of the page that sets this field which activates the export. 
    echo("<input type='hidden' name='export' value='' />");

    echo("<input type='hidden' name='show_quick' value='true' />");

    echo("<input type='hidden' name='start_row' value='" . $start_row . "' />\n");
    echo("<input type='hidden' name='per_page_max' value='" . $per_page_max . "' />\n");

    if ($count > $per_page_max) 
    {
        // The paging table which allows users to jump from page to page in their search results. 
        echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

        echo("<tr>");
        echo("<td>");
        echo($count . " contact" . (($count != 1) ? "s" : "") . " on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");
        echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
        echo(" contacts per page.");
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

            // This if else block controls what the user will see in the drop down menu in the paging table. For example,
            // they'll see one option as "Bart to Bertha", and "Burns to Byrd". The options they see in the drop down box
            // corresponds to what they want the results to be ordered by. 

            // There are two cases we have to handle. The first is an atypical case. Notice in display_array.inc that a 
            // user can select Company Name and Division Name and put them both into a single column. Usually the two 
            // columns would be separated into different columns, but staff need both columns in one column for some 
            // reason. Anyways, this drop down menu usually looks at the alias field of the display_array to determine 
            // what it needs to display. Notice that the alias field is called company_division_name, but there is no
            // alias in the select field of the display_array called company_division_name. So this if statement handles
            // this strange case. 
            if ($display_array[$searchContact->search_order[1]]["name"] == "Company Name, Division Name")
            {
                $contact_start = $arrRow[$row_num_start]["ec_company_name"];
                $contact_end = $arrRow[$row_num_end]["ec_company_name"];
            }
            elseif ($display_array[$searchContact->search_order[1]]["name"] == "Contact Address" 
                || $display_array[$searchContact->search_order[1]]["name"] == "Division Address"
                || $display_array[$searchContact->search_order[1]]["name"] == "Company Address"
                )
            {
                $tempOrderBy = $display_array[$searchContact->search_order[1]]["order"];
                $contact_start = $arrRow[$row_num_start][$tempOrderBy];
                $contact_end = $arrRow[$row_num_end][$tempOrderBy];
            }

            // Otherwise, all other cases are normal and we can look at the alias field of display_array to get the 
            // value we want. 
            else
            {
                // $display_array[$searchContact->search_order[1]]["alias"] looks nasty but it's not.
                // $searchContact->search_order[1] just gets the first thing we're going to order by.
                // "alias" will just get the column we're going to order by. So the $display_array[x][y]
                // will bring back something like "ec_company_name".
                // So $contact_start will equal something like $arrRow[1]["ec_company_name"]
                
                $contact_start = $arrRow[$row_num_start][$display_array[$searchContact->search_order[1]]["alias"]];
                $contact_end = $arrRow[$row_num_end][$display_array[$searchContact->search_order[1]]["alias"]];
            }

            if (strlen($contact_start) > 15) { $contact_start = substr($contact_start, 0, 13) . "..."; }

            if (strlen($contact_end) > 15) { $contact_end = substr($contact_end, 0, 13) . "..."; }

            echo("<option value='" . $row_num_start . "' ");
            if ($start_row == $row_num_start)
            {
                echo("selected='selected'");
            }
            echo(">" . $contact_start . " to " . $contact_end . "</option>\n");
        }

        echo("</select>");
        echo("</td>");

        echo("<td align='right'>");
        echo("<input type='submit' name='continue' value='View' />");
        echo("</td>");

        echo("</tr>");

        echo("<tr>");
        echo("<td align='center' colspan='2'>");
        echo("<hr />");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td align='center' colspan='2'>");
        echo("<input type='submit' name='continue' value='E-mail all Matches' />");
        echo ("&nbsp;<input type='submit' name='export' value='Export Mail Labels' />");
        echo("</td>");
        echo("</tr>");

        echo("</table>");
        echo("<br />");
    }

    $exstring = ""; 

    echo("<table cellspacing='0' cellpadding='0' border='1'>");
    echo("<tr>");
    echo("<td>");
    echo("<table border='0' cellpadding='2'>");
    echo("<tr>");
    echo("<td class='rowgrey' align='center'>&nbsp;</td>");

    $form_string = null;

    // print column headers
    for ($i=1;$i<=sizeof($searchContact->search_display);$i++)
    {
        echo("\n\n");
        echo("<td class='rowgrey' align='center' nowrap='nowrap'>\n");
        // This javascript call will submit the forms declared near the top of this presentation portion of the code. 
        if ($display_array[$searchContact->search_display[$i]]["name"] != "Contact Flags")
        {
            // These are forms used to dynamically sort columns in the search results. The forms have to be placed out here
            // because nested forms are illegal in HTML. 

            // When a column title is clicked, the results will order by that field. This form handles that.
            $form_string .= ("<form name='form".$i."' action='".$PHP_SELF."&amp;select=view_contact' method='post'>\n");
            $form_string .= ("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />\n");
            $form_string .= ("<input type='hidden' name='continue' value='re-sort' />\n");
            $form_string .= ("<input type='hidden' name='neworder' value='".$searchContact->search_display[$i]."' />\n");
            $form_string .= ("<input type='hidden' name='level1' value='contact' />\n");
            $form_string .= ("</form>\n");

            echo("<a href='javascript:document.form".$i.".submit()'>");
        }
        echo("&nbsp;<b class='white'>\n");
        echo($display_array[$searchContact->search_display[$i]]["name"]);
        echo("&nbsp;</b>");
        if ($display_array[$searchContact->search_display[$i]]["name"] != "Contact Flags")
        {
            echo("</a>");
        }
        echo("\n");
        echo("</td>");

        // Separate address columns into their own columns. ie Address 1.....N get their own columns. 
        if (is_array($display_array[$searchContact->search_display[$i]]["alias"]))
        {
            for ($k=0;$k<sizeof($display_array[$searchContact->search_display[$i]]["alias"]);$k++)
            {
                $number = $k + 1;
                $exstring .= $display_array[$searchContact->search_display[$i]]["name"]." ".$number."\t";
            }
        }
        else
        {
            $exstring .= $display_array[$searchContact->search_display[$i]]["name"]."\t";
        }
    }
    $exstring .= "\n";

//    echo("</td>");
//    echo("</tr>\n");

    $rowclass = 0;

    // Begin displaying results of the advanced search. This displayed the first x number of rows. 
	for ($i=$first;$i<=$end;$i++)
	{
		echo("<tr>\n");
		echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
		echo("<input type='radio' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "' name='contact_id' value='" . $arrRow[$i]["contact_id"] . "' onclick='submit()' />");
		echo("</td>");
        
        for ($j=1;$j<=sizeof($searchContact->search_display);$j++)
        {
			echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;");
            
            $display_index = $searchContact->search_display[$j];

            // If you look at display_array, you'll notice that sometimes the alias field
            // contains an array. Alias arrays imply that for a paticular column for the 
            // results page, multiple columns had to be selected from the database. Street
            // address is the typical case for this. We want to get a street address, so 
            // we fetch street_address_1 ... street_address_n from the database. The following
            // if statement will append the street information of a single contact/division/
            // company into a single string and display it in one column. 
            if (is_array($display_array[$display_index]["alias"]))
            {
                $address_string = "";
                for ($k=0;$k<sizeof($display_array[$display_index]["alias"]);$k++)
                {
                    $address_alias = $display_array[$display_index]["alias"][$k];
                    $address_string .= $arrRow[$i][$address_alias]. " ";
                }

                echo $address_string;
            }

            // E-mail fields get to be links in the results page. 
            elseif($display_array[$display_index]["alias"] == "c_email" ||
                   $display_array[$display_index]["alias"] == "ed_email" ||
                   $display_array[$display_index]["alias"] == "ec_email")
            {
                $email_alias = $display_array[$display_index]["alias"];
                echo("<a href='mailto:".$arrRow[$i][$email_alias]."'>");
                echo($arrRow[$i][$email_alias]);
                echo("</a>");
            }

            elseif($display_array[$display_index]["alias"] == "company_division_name")
            {
                if ($arrRow[$i]["ec_company_name"] == $arrRow[$i]["ed_department_name"])
                {
                    echo $arrRow[$i]["ec_company_name"];
                }
                else
                {
                    echo $arrRow[$i]["ec_company_name"].", ".$arrRow[$i]["ed_department_name"];
                }
            }

            elseif($display_array[$display_index]["alias"] == "cab_c_name")
            {
                echo $arrRow[$i]["cab_c_first_name"]." ".$arrRow[$i]["cab_c_last_name"];
            }

            // Websites also get to be links in the results page. 
            elseif($display_array[$display_index]["alias"] == "ed_website")
            {
                if (sizeof(trim($arrRow[$i]["ed_website"])) == 0)
                {
                    echo("&nbsp;");
                }
                else
                {
                    echo("<a href=\"http://".$arrRow[$i]["ed_website"]."\" target=\"_blank\">".$arrRow[$i]["ed_website"]."</a>");
                }
            }

            elseif($display_array[$display_index]["alias"] == "ec_website")
            {
                if (sizeof(trim($arrRow[$i]["ec_website"])) == 0)
                {
                    echo("&nbsp;");
                }
                else
                {
                    echo("<a href=\"http://".$arrRow[$i]["ec_website"]."\" target=\"_blank\">".$arrRow[$i]["ec_website"]."</a>");
                }
            }

            else
            {
                $row_alias = $display_array[$display_index]["alias"];
                echo $arrRow[$i][$row_alias];
            }

            echo("</td>");
        }// end for

		echo("</tr>\n");
		$rowclass++;
	}// end for

    // export to file string. this loop will export ALL rows from the result, not just the first x. 
	for ($i=0;$i<$count;$i++)
	{
        for ($j=1;$j<=sizeof($searchContact->search_display);$j++)
        {
            $display_index = $searchContact->search_display[$j];

            if (is_array($display_array[$display_index]["alias"]))
            {
                $address_string = "";
                for ($k=0;$k<sizeof($display_array[$display_index]["alias"]);$k++)
                {
                    $address_alias = $display_array[$display_index]["alias"][$k];
                    
                    // strip trailing tab character for last interation.
                    if ($k == (sizeof($display_array[$display_index]["alias"])-1))
                    {
                        $address_string .= $arrRow[$i][$address_alias];
                    }
                    else
                    {
                        $address_string .= $arrRow[$i][$address_alias]. "\t";
                    }
                }

                $exstring .= $address_string;
            }

            elseif($display_array[$display_index]["alias"] == "c_email" ||
                   $display_array[$display_index]["alias"] == "ed_email" ||
                   $display_array[$display_index]["alias"] == "ec_email")
            {
                $email_alias = $display_array[$display_index]["alias"];
                $exstring .= $arrRow[$i][$email_alias];
            }

            elseif($display_array[$display_index]["alias"] == "company_division_name")
            {
                if ($arrRow[$i]["ec_company_name"] == $arrRow[$i]["ed_department_name"])
                {
                    $exstring .= $arrRow[$i]["ec_company_name"];
                }
                else
                {
                    $exstring .= $arrRow[$i]["ec_company_name"].", ".$arrRow[$i]["ed_department_name"];
                }
            }

            elseif($display_array[$display_index]["alias"] == "cab_c_name")
            {
                $exstring .= $arrRow[$i]["cab_c_first_name"]." ".$arrRow[$i]["cab_c_last_name"];
            }

            else
            {
                $row_alias = $display_array[$display_index]["alias"];
                $exstring .= $arrRow[$i][$row_alias];
            }

            $exstring .= "\t";
        }//end for

        $exstring .= "\n";
	}//end for

	echo("<tr>");
		echo("<td colspan='" . (sizeof($searchContact->search_display) + 1) . "' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("<input type='submit' value='View Selected Contact' />");
        echo("<input type='hidden' name='export_string' value='" . packObject(($exstring)) . "' />");
		echo("</td>\n");
	echo("</tr>\n");

	echo("</table>\n");
	echo("</td>\n");
	echo("</tr>\n");
	echo("</table>\n");
    echo("</form>");

    echo("<table border='0' cellspacing='0' cellpadding='0'>");
    echo("<tr>");
    echo("<td>");
    echo("<input type='button' value='Export to File' onclick='javascript:triggerExport(\"main_form\",\"export\",\"Export to File\")' /> ");
    echo("<input type='button' value='Export Mail Labels' onclick='javascript:triggerExport(\"main_form\",\"export\",\"Export Mail Labels\")' /> ");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td>");
    echo("&nbsp;");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td>");
    echo("<input type='button' value='Set Actions for Matched Contacts' onclick='javascript:document.set_actions.submit()' /> ");
    echo("<input type='button' value='Set Flags for Matched Contacts' onclick='javascript:document.set_flags.submit()' /> ");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td>");
    echo("&nbsp;");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td>");
    echo("<input type='button' value='E-mail all Matches' onclick='javascript:document.email_all_form.submit()' /> ");
    echo("</td>");
    echo("</tr>");

    if (strlen($contact_id_list)) {
        // plugins
        // if you want to do anything else with the results, just use the following hook
        $arr_params = array();
        $arr_params['contact_id_list'] = $contact_id_list;
        $arr_params['PHP_SELF'] = $PHP_SELF;

        $arr_buttons  = get_hooks('mamook.contact_search_results.button', $arr_params);
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

    echo("</table>");

    if (strlen($contact_id_list)) {
        // display the forms of the buttons from the plugins need
        if (is_array($arr_buttons) && sizeof($arr_buttons)) {
            for ($i = 0; $i < sizeof($arr_buttons); $i++) {
                echo($arr_buttons[$i]['form']);
            }
        }
    }

    echo("<form method='post' name='set_actions' action='" . $PHP_SELF . "'>\n");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
    echo("<input type='hidden' name='select' value='Set Actions for Matched Contacts' />");
    echo("<input type='hidden' name='PDF' value='1' />");
    echo("</form>");

    echo("<form method='post' name='set_flags' action='" . $PHP_SELF . "'>\n");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
    echo("<input type='hidden' name='select' value='Set Flags for Matched Contacts' />");
    echo("<input type='hidden' name='PDF' value='1' />");
    echo("</form>");

    echo("<form method='post' action='" . $PHP_SELF . "' name='email_all_form'>\n");
    echo("<input type='hidden' name='select' value='view_contact' />\n");
    echo("<input type='hidden' name='level1' value='contact' />\n");
    echo("<input type='hidden' name='continue' value='E-mail all Matches' />\n");
    echo("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />\n");
    echo("<input type='hidden' name='contact_id_string' value=\"" . packObject($contact_id_list) . "\" />");
    echo("<input type='hidden' name='show_quick' value='true' />");
    echo("</form>");

    //echo("<form method='post' action='".$PHP_SELF."&amp;select=contacts_pdf_batch'>");
    //echo("<input type='hidden' name='contact_id_string' value='".packObject($contact_id_string)."' />");
    //echo("<input type='submit' value='Export Records to PDF' />");
	//echo("</form>");
   
    // spit out the dynamic column sorting forms here 
    echo($form_string);

    ?>
        <script type='text/javascript' language='javascript'>
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

?>
