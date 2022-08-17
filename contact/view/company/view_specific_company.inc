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
 | Filename: view_specific_company.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is used to display all of the attributes for a        |
 | company.  The form can be displayed without buttons, in which case the user  |
 | is given the option of viewing the information in PDF (for easy printing).   |
 +------------------------------------------------------------------------------+

*/

// :TODO: This file doesn't do PDF viewing any more. Remove all comments about this. 

if(strcmp($message_sent,"yes") == 0)
{
    echo "<br />".notify("The e-mail has been sent successfully.")."<br />";
    $message_sent == "";
}
$sql = ("
        SELECT CURRENT_DATE as today
        ");
$date_result = $GLOBALS['dbh']->Execute($sql);
$date_row = $date_result->FetchRow();
$todays_date = $date_row["today"];

if ($departments_match)
{
	$msg .= ("There were divisions in the database that matched the criteria you searched on.  Click the button below to view these divisions.");
    $msg .= ("<br /><br />");
    $msg .= ("<input type='hidden' name='select' value='view_contact' />");
    $msg .= ("<input type='hidden' name='level1' value='company' />");
    $msg .= ("<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />");
    $msg .= ("<input type='hidden' name='continue' value='view_division_search_results' />");
    $msg .= ("</b><center><input type='submit' value='View Divisions Returned' /></center><b>&nbsp;");
    echo("<form method='post' action='" . $PHP_SELF . "'>");
    instruct($msg);
    echo("</form>");
	
	echo("<br />");
}


if (!$no_buttons)
{

	/*
	 Figure out where this division's department_id lies in the department_id_list string (that has been passed to this script)
	 and then find the left and right department_id_list for the links at the bottom and top of the page.
	*/

	$employer_id_array = explode(",", trim($employer_id_list));
	 
	for ($i = 0; $i < sizeof($employer_id_array); $i++)
	{
		if ($contact->employer_id == $employer_id_array[$i])
		{
			$position = $i;
			$left_pos = $i - 1;
			$right_pos = $i + 1;
			break;
		}
	}

	if ($left_pos < 0)
	{
		$left_pos = (sizeof($employer_id_array) - 1);
	}
	if ($right_pos > (sizeof($employer_id_array) - 1))
	{
		$right_pos = 0;
	}

	$left_id = trim($employer_id_array[$left_pos]);
	$right_id = trim($employer_id_array[$right_pos]);

	// Display the forward and backward buttons at top of page.

	$formStart .= ("
		<center>
		<table width='96%' border='0'>
		<tr>
		<td align='left'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='company' />
		<input type='hidden' name='continue' value='view_specific_company' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
		<input type='hidden' name='employer_id_list' value='" . $employer_id_list . "' />
		<input type='hidden' name='employer_id' value='" . $left_id . "' />
		<input type='hidden' name='departments_match' value='" . $departments_match . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='image'  src='misc/images/previous.gif' />
		</form>
		</td>
		<td align='center'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='company' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
		<input type='submit' name='continue' value='Back to List' />
		</form>
		</td>
		<td align='right'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='company' />
		<input type='hidden' name='continue' value='view_specific_company' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
		<input type='hidden' name='employer_id_list' value='" . $employer_id_list . "' />
		<input type='hidden' name='employer_id' value='" . $right_id . "' />
		<input type='hidden' name='departments_match' value='" . $departments_match . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='image'  src='misc/images/next.gif' />
		</form>
		</td>
		</tr>
		</table>
		</center>
		");
}


$companyOutput .= ("<center>");

/*
 Open up the table that everything will be displayed in.  This is the main table that holds everything.
*/

$companyOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='96%'>");
$companyOutput .= ("<tr>");
	$companyOutput .= ("<td align='center' valign='middle' colspan='2'>");

	/*
	 Secondary table below is used to avoid ugly lines all over the place.
	*/

	$companyOutput .= ("<table border='0' align='center' width='100%'>");
	$companyOutput .= ("<tr>");
	$companyOutput .= ("<td colspan='2' align='center'>");
		$companyOutput .= ("<h3>");
		$companyOutput .= ("<br />");
		$companyOutput .= ($contact->company_name);
		$companyOutput .= ("<br />");
		$companyOutput .= ("</h3>");
	$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
	
	$companyOutput .= ("<tr>");
	        $companyOutput .= ("<td colspan='2'><hr /></td>");
	$companyOutput .= ("</tr>");
	
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

if ($contact->company_phone)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td align='right' width='50%' nowrap='nowrap'><b>Company Phone:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		$companyOutput .= ($contact->company_phone);
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_fax)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Fax:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		$companyOutput .= ($contact->company_fax);
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_email)
{
    if(is_string($searchContact)){
        $searchContact = unpackObject($searchContact); 
    }
    
	$companyOutput .= ("<tr align='center'>");    
    $companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company E-mail:</b></td>");
    $companyOutput .= ("<form name='emailform1' method='post' action='".$PHP_SELF."'>");
    $companyOutput .= ("<input type='hidden' name='after' value='contact/view/company/view_company.inc' />");
    $companyOutput .= ("<input type='hidden' name='to' value='".$contact->company_email."' />");
    $companyOutput .= ("<input type='hidden' name='employer_id' value='".$contact->employer_id."' />");
    if(isset($searchContact)){
        $companyOutput .= ("<input type='hidden' name='employer_id_list' value='".$employer_id_list."' />");
        $companyOutput .= ("<input type='hidden' name='searchContact' value='".packObject($searchContact)."' />");
    }
    $companyOutput .= ("<input type='hidden' name='no_buttons' value='".$no_buttons."' />");
    $companyOutput .= ("<input type='hidden' name='select' value='set_company_mail' />");
    $companyOutput .= ("<td width='50%' align='left'>");
    $companyOutput .= ("<a href='javascript:document.emailform1.submit()'>". $contact->company_email . "</a>");
    $companyOutput .= ("</td>");
    $companyOutput .= ("</form>");
    //$companyOutput .= ("<td width='50%' align='left'><a href='mailto:" . $contact->company_email . "'>" . $contact->company_email . "</a></td>");
    $companyOutput .= ("</tr>");
}

if ($contact->company_website)
{
    $companyOutput .= ("<tr align='center'>");
    $companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Website:</b></td>");
    $companyOutput .= ("<td width='50%' align='left'>");
    $companyOutput .= ("<a class='blue' target='company_website' href='http://" . stripSlashes($contact->company_website) . "'>" . stripSlashes($contact->company_website) . "</a>");
    $companyOutput .= ("</td>");
    $companyOutput .= ("</tr>");
}

if ($contact->company_size_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Size:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getSizeRange($contact->company_size_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_type_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Type:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getCompanyTypeName($contact->company_type_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_industry_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Industry:</b></td>");		
		$companyOutput .= ("<td width='50%' align='left'>" . getIndustryName($contact->company_industry_id) . "</td>");
	$companyOutput .= ("</tr>");
}

$companyOutput .= ("<tr align='center'>");
	$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
$companyOutput .= ("</tr>");

if ($contact->company_street_address_1)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Street Address:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_street_address_1);
		if ($contact->company_street_address_2)
		{
			$companyOutput .= ("<br />" . $contact->company_street_address_2);
		}
		if ($contact->company_street_address_3)
		{
			$companyOutput .= ("<br />" . $contact->company_street_address_3);
		}
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_country_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Country:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getCountryName($contact->company_country_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_provstate_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Province/State:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getProvstateName($contact->company_provstate_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_region_id)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Region:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . getRegionName($contact->company_region_id) . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_city)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>City:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_city . "</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_postal_code)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Postal/Zip Code:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>" . $contact->company_postal_code . "</td>");
	$companyOutput .= ("</tr>");
}


$companyOutput .= ("<tr align='center'>");
	$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
$companyOutput .= ("</tr>");

// Bring me a status for each department that posts jobs (job_list = 1).
$sql = ("
        SELECT d.department_name, eis.status_name
        FROM department_company_status dcs
        INNER JOIN employer_info_statuses eis
        ON dcs.status_id = eis.status_id
        INNER JOIN department d
        ON dcs.department_id = d.department_id
        WHERE dcs.employer_id = '".$contact->employer_id."' AND d.job_list = '1' AND dcs.status_id != '".CONTACT_POTENTIAL_LEAD."' AND dcs.current_status = '1'
        ORDER BY d.department_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

$status_array = array();
if ($result->RecordCount())
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td align='center' colspan='2'>");
		$companyOutput .= ("<b>This company has the following statuses with the departments listed below:</b>");
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");

    while ($row = $result->FetchRow())
    {
		$companyOutput .= ("<tr align='center'>");
			$companyOutput .= ("<td align='right' width='50%'>");
			$companyOutput .= ("<b>" . $row["department_name"] . ":</b>");
			$companyOutput .= ("</td>");
			$companyOutput .= ("<td align='left' width='50%'>");
			$companyOutput .= ("<b class='black'>" . $row["status_name"] . "</b>");
			$companyOutput .= ("</td>");
		$companyOutput .= ("</tr>");
    }                                                                                                                                                

	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");
}

if ($contact->company_flags)
{
	$companyOutput .= ("<tr align='center'>");
		$companyOutput .= ("<td width='50%' align='right' nowrap='nowrap'><b>Company Flags that<br />have been Checked:</b></td>");
		$companyOutput .= ("<td width='50%' align='left'>");
		if (sizeof($contact->company_flags) == 1)
		{
			$companyOutput .= (getFlagName($contact->company_flags[0], "company"));
		}
		else
		{
			for ($i = 0; $i < (sizeof($contact->company_flags) - 1); $i++)
			{
				$companyOutput .= (getFlagName($contact->company_flags[$i], "company") . ", ");
			}
			$companyOutput .= (getFlagName($contact->company_flags[$i], "company"));
		}
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

}

$companyOutput .= ("<tr>");
	$companyOutput .= ("<td width='50%'>&nbsp;</td>");
	$companyOutput .= ("<td width='50%'>&nbsp;</td>");
$companyOutput .= ("</tr>");

if ($contact->company_description)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td width='50%' align='right'><b>Company Description:</b></td>");
		$companyOutput .= ("<td>");
		
		$temp = split("\n", $contact->company_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$companyOutput .= ($temp[$i]);
			$companyOutput .= ("<br />");
		}

		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

}

$sql = ("
	SELECT department_id
	FROM employer_department
	WHERE employer_id='" . $contact->employer_id . "'
	ORDER BY department_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

$division_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=company&amp;continue=view_specific_department&amp;show_quick=true&amp;no_buttons=true&amp;department_id=");

if ($result->RecordCount())
{
    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td colspan='2'><hr /></td>");
    $companyOutput .= ("</tr>");

    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td align='center' colspan='2'><b>Divisions at this company:</b></td>");
    $companyOutput .= ("</tr>");

    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td align='center' colspan='2'>");
    $companyOutput .= ("<table border='0'><tr><td>");

    while ($row = $result->FetchRow())
    {
        $sql = ("
                SELECT DISTINCT city
                FROM employer_department
                WHERE department_id='" . $row["department_id"] . "'
                ");
        $result2 = $GLOBALS['dbh']->Execute($sql);
        $row2 = $result2->FetchRow();

            $sql3 = ("
                    SELECT status_id
                    FROM department_division_status
                    WHERE division_id='" . $row["department_id"] . "'
                    AND department_id = '".$auth->department."'
                    AND current_status = 1
                    ");
            $result3 = $GLOBALS['dbh']->Execute($sql3);
            $row3 = $result3->FetchRow();

            if($row3["status_id"] <> CONTACT_POTENTIAL_LEAD)
            {
                $division_status = "(".getEmployerStatusName($row3["status_id"]).")";
                $companyOutput .= ("<a class='blue' href='" . $division_href . $row["department_id"] . "'><b>" . getDivisionName($row["department_id"]) . "</b></a>");
                $companyOutput .= ($row2["city"] ? " (" . $row2["city"] . ")" : "");
                $companyOutput .= (" ".$division_status);
            } else {
                $companyOutput .= ("<a class='blue' href='" . $division_href . $row["department_id"] . "'>" . getDivisionName($row["department_id"]) . "</a>");
                $companyOutput .= ($row2["city"] ? " (" . $row2["city"] . ")" : "");
            }
        $companyOutput .= ("\n<br />");
    }

    $companyOutput .= ("</td></tr></table>");
    $companyOutput .= ("</td>");
    $companyOutput .= ("</tr>\n");
}

$sql_actions = ("
        SELECT cas.action_id, cas.contact_id, cas.action_by, cas.action_on
        , cas.action_method_id, cas.action_note, cas.bring_forward_date
        FROM contact_actions_set AS cas
        INNER JOIN contact_employer AS ce ON ce.contact_id=cas.contact_id
        WHERE ce.employer_id = '" .$contact->employer_id. "'
        AND cas.action_id != '". GENERIC_NOTE ."'
        ORDER BY cas.action_on DESC, cas.contact_actions_id DESC
        LIMIT 10
        ");
$result_actions = $GLOBALS['dbh']->Execute($sql_actions);

if ($result_actions->RecordCount())
{
    $companyOutput .= ("</table>\n");
    $companyOutput .= ("<hr />");
    $companyOutput .= ("<table cellpadding='5' cellspacing='0' border='0' align='center'>\n");
    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td colspan='6'>&nbsp;</td>");
    $companyOutput .= ("</tr>\n");
    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td align='center' colspan='6'>");
    $companyOutput .= ("Ten Most Recent Actions for ".$contact->company_name.": ");
    $companyOutput .= ("</td>");
    $companyOutput .= ("</tr>\n");
    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td align='center' colspan='6'>&nbsp;</td>");
    $companyOutput .= ("</tr>\n");

    $count = 0; // keep a counter to identify the actions displayed
    while ($row_actions = $result_actions->FetchRow())
    {
        // Get action name
        $mini_sql = ("
                SELECT brief_action_name
                FROM action_types
                WHERE action_id = '" . $row_actions['action_id']. "'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $action_name = $mini_row["brief_action_name"];

        // Get action method name
        // Note: action methods introduced at later development date, thus some actions methods were "Never Set"
        $mini_sql =("
                SELECT action_method_name
                FROM action_methods
                WHERE action_method_id = '" .$row_actions["action_method_id"]. "'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $action_method_name = ($mini_row["action_method_name"]!="" ? $mini_row["action_method_name"] : "Never Set");

        // get name of action_for
        $mini_sql = ("
                SELECT ed.department_name AS name
                FROM employer_department AS ed
                INNER JOIN contact_employer AS ce ON ce.department_id=ed.department_id
                WHERE ce.contact_id='".$row_actions['contact_id']."'
                ");
        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
        $mini_row = $mini_result->FetchRow();
        $display_name_for = $mini_row['name'];

        // Display our query results
        $companyOutput .= ("<tr>");
        // Display bring_forward_date in pop-up form if this action had one
        if(($row_actions["bring_forward_date"]!='0000-00-00') && ($row_actions["bring_forward_date"] != NULL) && ($row_actions["bring_forward_date"] > $todays_date))
        {
            $companyOutput .= ("<td nowrap='nowrap'>");

            // pop-up function call
            $companyOutput .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('".$row_actions['bring_forward_date']."', CAPTION, 'Bring Forward Date', LEFT, VAUTO, STICKY, ol_closeclick);\" onmouseout=\"nd();\">");
            $companyOutput .= ("<img border='0' src='misc/images/clock.gif' alt='' />");
            $companyOutput .= ("</a>");
            $companyOutput .= ("</td>\n");
        }
        else
        {
            $companyOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
        }

        // Display note in pop-up form if this action had one
        if($row_actions["action_note"])
        {
            $companyOutput .= ("<td nowrap='nowrap'>");
            $contact_action_note = trim($row_actions['action_note']);
            $contact_action_note = addslashes($contact_action_note);

            //  newlines, carriage returns, and double quotes passed into overlib() function must be changed to work properly
            $contact_action_note = str_replace("\"", "'", $contact_action_note);
            $contact_action_note = str_replace("\r", "", $contact_action_note);
            $contact_action_note = str_replace("\n", "<br />", $contact_action_note);

            // pop-up function call
            $companyOutput .= ("<a href='javascript:void(0);' onmouseover=\"return overlib('".$contact_action_note."', CAPTION, 'Action Note', LEFT, VAUTO, STICKY, ol_closeclick);\" onmouseout=\"nd();\">");
            $companyOutput .= ("<img border='0' src='misc/images/sticky_sm.gif' alt='' />");
            $companyOutput .= ("</a>");
            $companyOutput .= ("</td>\n");
        }
        else
        {
            $companyOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
        }

        //date of action
        $companyOutput .= ("<td nowrap='nowrap'>");
        if ($row_actions['action_on'] == '0000-00-00' || $row_actions['action_on'] == NULL)
        {
            $companyOutput .= ("<b class='black'>On:</b> " . formatShortDate($row_actions["entered_on"]));
        } else {
            $companyOutput .= ("<b class='black'>On:</b> " . formatShortDate($row_actions["action_on"]));
        }
        $companyOutput .= ("</td>");
        $date[$count] = formatShortDate($row_actions["action_on"]);

        //action chosen
        $companyOutput .= ("<td nowrap='nowrap'>");
        $companyOutput .= ("<b class='black'>Action:</b> " . $action_name);
        $companyOutput .= ("</td>");

        //action performed by/for
        $companyOutput .= ("<td nowrap='nowrap'>");
        $companyOutput .= ("<b class='black'>For: </b>".$display_name_for);
        $companyOutput .= ("</td>\n");

        // method of action (do not display method column if action is JOB_DESC_RECIEVED)
        $companyOutput .= ("<td nowrap='nowrap'>");
        $companyOutput .= (($row_actions["action_id"] == JOB_DESC_RECEIVED)? "&nbsp;" : "<b class='black'>Method:</b> " . $action_method_name);
        $companyOutput .= ("</td>\n");

        $companyOutput .= ("</tr>\n");
    }
    $companyOutput .= ("</table>");
    $companyOutput .= ("<table width='100%' border='0'>");
}

if ($contact->company_note_ids)
{
    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td colspan='2'><hr /></td>");
    $companyOutput .= ("</tr>");

    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td colspan='2'>");
    $companyOutput .= ("<table width='100%' border='0'>");

    foreach ($contact->company_note_ids AS $cni)
    {
        $sql = ("
                SELECT DISTINCT *
                FROM company_notes
                WHERE note_id='" . $cni . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $name_array = getContactName($row["entered_by"]);
        $contact_name = ($row["entered_by"] ? $contact_name = $name_array["first_name"] . " " . $name_array["last_name"] : "&nbsp;");

        $companyOutput .= ("<tr>");
        $companyOutput .= ("<td align='left'>");
        $companyOutput .= ("<b class='blue'>Note entered by: </b>");
        $companyOutput .= ("</td>");
        $companyOutput .= ("<td align='left'>");
        $companyOutput .= ($contact_name ? $contact_name : "&nbsp;");
        $companyOutput .= ("</td>");
        $companyOutput .= ("</tr>");
        $companyOutput .= ("<tr>");
        $companyOutput .= ("<td align='left'>");
        $companyOutput .= ("<b>Note entered on: </b>");
        $companyOutput .= ("</td>");
        $companyOutput .= ("<td align='left'>");
        $companyOutput .= ($row["date_entered"]);
        $companyOutput .= ("</td>");
        $companyOutput .= ("</tr>");
        $companyOutput .= ("<tr>");
        $companyOutput .= ("<td align='left'>");
        $companyOutput .= ("<b>Note: </b>");
        $companyOutput .= ("</td>");
        $companyOutput .= ("<td align='left'>");

        $temp = split("\n", $row["notes"]);

        for ($i = 0; $i < sizeof($temp); $i++)
        {
            $companyOutput .= ($temp[$i]);
            $companyOutput .= ("<br />");
        }

        $companyOutput .= ("</td>");
        $companyOutput .= ("</tr>");

        $companyOutput .= ("<tr>");
        $companyOutput .= ("<td colspan='2'>&nbsp;</td>");
        $companyOutput .= ("</tr>");
    }

    $companyOutput .= ("</table>");
    $companyOutput .= ("</td>");
    $companyOutput .= ("</tr>");

}

/*
if ($contact->company_general_comments)
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='right'><b>Comments viewed<br />by all co-op staff:</b></td>");
		$companyOutput .= ("<td><br />");
	
		$temp = split("\n", $contact->company_general_comments);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$companyOutput .= ($temp[$i]);
			$companyOutput .= ("<br />");
		}

		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td colspan='2'>&nbsp;</td>");
	$companyOutput .= ("</tr>");

}
*/

if ($contact->company_department_comments)
{
    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td colspan='2'><hr /></td>");
    $companyOutput .= ("</tr>");

    $companyOutput .= ("<tr>");
    $companyOutput .= ("<td align='right'><b>Comments viewed by co-op<br />staff in your department:</b></td>");
    $companyOutput .= ("<td>");
    $companyOutput .= ("<br />");

    $temp = split("\n", $contact->company_department_comments);

    for ($i = 0; $i < sizeof($temp); $i++)
    {
        $companyOutput .= ($temp[$i]);
        $companyOutput .= ("<br />");
    }

    $companyOutput .= ("</td>");
    $companyOutput .= ("</tr>");

}


$sql = ("
	SELECT DISTINCT ce.contact_id, c.first_name, c.last_name
	FROM contact_employer AS ce, contact AS c
	WHERE ce.employer_id='" . $contact->employer_id . "'
	AND (ce.department_id='' OR ce.department_id IS NULL)
	AND ce.contact_id=c.contact_id
	ORDER BY c.last_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

$contact_href = ($PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;show_quick=true&amp;no_buttons=true&amp;contact_id=");

if ($result->RecordCount())
{
	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='center' colspan='2'><b>Contacts directly under this company:</b></td>");
	$companyOutput .= ("</tr>");

	$companyOutput .= ("<tr>");
		$companyOutput .= ("<td align='center' colspan='2'>");
		$companyOutput .= ("<table border='0'><tr><td>");
		
		while ($row = $result->FetchRow())
		{
			$companyOutput .= ("<a class='blue' href='" . $contact_href . $row["contact_id"] . "'>" . $row["first_name"] . " " . $row["last_name"] . "</a>");
			$companyOutput .= ("\n<br />");
		}

		$companyOutput .= ("</td></tr></table>\n");
		$companyOutput .= ("</td>");
	$companyOutput .= ("</tr>\n");
}

if ($contact->company_changes_recorded_1)
{
        $companyOutput .= ("<tr>");
                $companyOutput .= ("<td colspan='2'><hr /></td>");
        $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='center' colspan='2'>");
                        $companyOutput .= ("<b class='black'>Recent history of changes for this company:</b>");
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td width='50%'>&nbsp;</td>");
                        $companyOutput .= ("<td width='50%'>&nbsp;</td>");
                $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Fields changed:</b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= ($contact->company_changes_recorded_1);
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Changes above made on:</b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= (formatLongDate($contact->company_change_date_1));
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                        FROM contact AS a, department AS b, contact_internal AS c
                        WHERE a.contact_id='" . $contact->company_change_by_1 . "'
                        AND c.contact_id=a.contact_id
                        AND b.department_id=c.department_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $companyOutput .= ("<tr>");
                        $companyOutput .= ("<td align='right'>");
                        $companyOutput .= ("<b>Changes above made by:</b>");
                        $companyOutput .= ("</td>");
                        $companyOutput .= ("<td align='left'>");
                        $companyOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                        $companyOutput .= ("</td>");
                $companyOutput .= ("</tr>");

                if ($contact->company_changes_recorded_2)
                {
                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
				$companyOutput .= ("<b>Fields changed:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($contact->company_changes_recorded_2);
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made on:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= (formatLongDate($contact->company_change_date_2));
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
                                WHERE a .contact_id='" . $contact->company_change_by_2 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made by:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");
                }
                if ($contact->company_changes_recorded_3)
                {
                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td colspan='2'>&nbsp;</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Fields changed:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($contact->company_changes_recorded_3);
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made on:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= (formatLongDate($contact->company_change_date_3));
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");

                        $sql = ("
                                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_code
                                FROM contact AS a, department AS b, contact_internal AS c
				WHERE a .contact_id='" . $contact->company_change_by_3 . "'
                                AND c.contact_id=a.contact_id
                                AND b.department_id=c.department_id
                               ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();

                        $companyOutput .= ("<tr>");
                                $companyOutput .= ("<td align='right'>");
                                $companyOutput .= ("<b>Changes above made by:</b>");
                                $companyOutput .= ("</td>");
                                $companyOutput .= ("<td align='left'>");
                                $companyOutput .= ($row["name"] . "&nbsp;(" . $row["department_code"] . ")");
                                $companyOutput .= ("</td>");
                        $companyOutput .= ("</tr>");
                }
}

/*
 Next is the last part of the screen that will be displayed.  We declare it now to get it out of the way.
 If we are showing form buttons, then this needs to be added after those buttons have been displayed, but
 if we are viewing in PDF, it needs to be added before the buttons (actually, the buttons aren't even
 displayed when viewing in PDF).
*/

$companyOutput.= ("</table>");
$companyOutput .= ("</td>");
$companyOutput .= ("</tr>");
$companyOutput .= ("</table>");
$companyOutput .= ("</center>");

/*
 And that's the end of the companyOutput variable.  Now we must form the formEnd variable, which will be displayed
 on this screen, but not when viewing the information in PDF.
*/

if (!$no_buttons)
{

	$formEnd .= ("
		<center>
		<table width='96%' border='0'>
		<tr>
		<td align='left'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='company' />
		<input type='hidden' name='continue' value='view_specific_company' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
		<input type='hidden' name='employer_id_list' value='" . $employer_id_list . "' />
		<input type='hidden' name='employer_id' value='" . $left_id . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='hidden' name='departments_match' value='" . $departments_match . "' />
		<input type='image'  src='misc/images/previous.gif' />
		</form>
		</td>
		<td align='center'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='company' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
		<input type='submit' name='continue' value='Back to List' />
		</form>
		</td>
		<td align='right'>
		<form method='post' action='" . $PHP_SELF . "'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='company' />
		<input type='hidden' name='continue' value='view_specific_company' />
		<input type='hidden' name='searchContact' value='" . packObject($searchContact) . "' />
		<input type='hidden' name='employer_id_list' value='" . $employer_id_list . "' />
		<input type='hidden' name='employer_id' value='" . $right_id . "' />
		<input type='hidden' name='show_quick' value='true' />
		<input type='hidden' name='departments_match' value='" . $departments_match . "' />
		<input type='image'  src='misc/images/next.gif' />
		</form>
		</td>
		</tr>
		</table>
		</center>
		");
}
echo($formStart . $companyOutput . $companyEnd . $formEnd);
