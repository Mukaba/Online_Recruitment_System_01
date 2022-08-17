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
 | Filename: view_contact.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file does all of the grunt work.  It deteremines which     |
 | form needs to be loaded for the user, and loads that form appropriately.     |
 +------------------------------------------------------------------------------+

*/

//:TODO Show ALL of the fields, regardless of whether or not they have been set.

include('contact/view/contact/SearchContact.class');

if ($switch_search)
{
	$continue = $switch_search;
}

if (!$continue)
{
		
	// They haven't done anything from here yet, give them the criteria on what to display.

    $sql = ("
        SELECT search_use_advanced_emp 
        FROM contact_internal 
        WHERE contact_id='" . $auth->contact_id . "'
        ");
    $response = $GLOBALS['dbh']->Execute($sql);
    
    if (!$response->RecordCount())
    {
        include ('contact/view/contact/view_contact_criteria_simple.inc');
    }
    else if (($array = $response->FetchRow()) && $array['search_use_advanced_emp'] == 1)
    {
        include ('contact/view/contact/view_contact_criteria.inc');
    }
    else
    {
        include ('contact/view/contact/view_contact_criteria_simple.inc');
    }
}                       
elseif ($continue == 'simple_contact_form_done')
{                       
	if (sizeof($error_array) == 0)
	{
		if (trim($search_first_name_m))
		{
            $search_first_name = $search_first_name_m;
		}

		if (trim($search_last_name_m))
		{
            $search_last_name = $search_last_name_m;
		}
		
        $temp_column = array();
        if (sizeof($column) && is_array($column))
        {
            $x = 1;
            for ($i=0;$i<sizeof($column);$i++)
            {
                if ($column[$i])
                {
                    $temp_column[$x++] = $column[$i];
                }
            }
            $column = $temp_column;
            unset($temp_column);
        }

        $temp_order = array();
        if (sizeof($order) && is_array($order))
        {
            $x = 1;
            for ($i=0;$i<sizeof($order);$i++)
            {
                if ($order[$i])
                {
                    $temp_order[$x++] = $order[$i];
                }
            }
            $order = $temp_order;
            unset($temp_order);
        }

		$searchContact = new SearchContact('');

		$searchContact->set_contact_values_1($search_first_name, $search_last_name, $search_email_as, $search_email, $search_phone_as
            , $search_phone,  $search_cellphone_as, $search_cellphone, $search_fax_as, $search_fax, $search_company_name, $search_division_name, $search_area
            , $search_country, $search_provstate, $search_region, $search_city, $search_postal_code, $search_action, $search_action_by
            , $search_action_as, $search_action_on, $search_action_between1, $search_action_between2, $search_action_bf_as, $search_action_bf_on
            , $search_action_bf_between1, $search_action_bf_between2, $search_flags_boolean, $checked_flags, $checked_nflags, $search_department_name
            , $search_contact_status_status, $search_contact_status_department, $column, $order, $search_company_criteria
            , $search_department_criteria
			);

        $error_array = $searchContact->checkInputs();

        if (sizeof($error_array) > 0)
        {
            include('contact/view/contact/view_contact_criteria_simple.inc');
        }
        
		else
		{
			include('contact/view/contact/view_contact_search_results.inc');
		}
	}
}
elseif ($continue == 'first_contact_form_done')
{                       
    if ($less_orders || $more_orders || $less_columns || $more_columns)
    {
        include("contact/view/contact/view_contact_criteria.inc");
    }

	elseif (sizeof($error_array) == 0)
	{
		if (trim($search_first_name_m))
		{
            $search_first_name = $search_first_name_m;
		}

		if (trim($search_last_name_m))
		{
            $search_last_name = $search_last_name_m;
		}
		
        // Replace "s with 's in the names
        $search_first_name = preg_replace("/\"/", "'", $search_first_name);
        $search_last_name = preg_replace("/\"/", "'", $search_last_name);

        $temp_column = array();

        if (sizeof($column) && is_array($column))
        {
            $x = 1;
            for ($i=0;$i<sizeof($column)+1;$i++)
            {
                if ($column[$i])
                {
                    $temp_column[$x++] = $column[$i];
                }
            }
            $column = $temp_column;
            unset($temp_column);
        }

        $temp_order = array();
        if (sizeof($order) && is_array($order))
        {
            $x = 1;
            for ($i=0;$i<sizeof($order)+1;$i++)
            {
                if ($order[$i])
                {
                    $temp_order[$x++] = $order[$i];
                }
            }
            $order = $temp_order;
            unset($temp_order);
        }

        $searchContact = new SearchContact('');
		$searchContact->set_contact_values_1($search_first_name, $search_last_name, $search_email_as, $search_email, $search_phone_as
            , $search_phone, $search_cellphone_as, $search_cellphone, $search_fax_as, $search_fax, $search_company_name, $search_division_name, $search_area
            , $search_country, $search_provstate, $search_region, $search_city, $search_postal_code, $search_action, $search_action_by
            , $search_action_as, $search_action_on, $search_action_between1, $search_action_between2, $search_action_bf_as
            , $search_action_bf_on, $search_action_bf_between1, $search_action_bf_between2, $search_flags_boolean, $checked_flags, $checked_nflags
            , $search_department_name, $search_contact_status_status, $search_contact_status_department
            , $column, $order, $search_company_criteria, $search_department_criteria
			);

        $error_array = $searchContact->checkInputs();

        if (sizeof($error_array) > 0)
        {
            include('contact/view/contact/view_contact_criteria.inc');
        }
        
		elseif ($searchContact->search_company_criteria || $searchContact->search_department_criteria)
		{
			include('contact/view/contact/view_company_criteria.inc');
		}
		else
		{
            if($btnSearch=="Save Search")
            {
                $search_saved = true;
                $module_id = CONTACT_ADV_SEARCH;
                if (!is_string($searchContact))
                {
                    $search = packObject($searchContact);
                }
                if($edit_page)
                {
                    $continue = "Edit Search Criteria";
                }
                include_once("search/save_search.inc");
            }
            else
            {
                include('contact/view/contact/view_contact_search_results.inc');
            }
        }
    }
}
elseif($continue == 'save_contact_search')
{
    $module_id = CONTACT_ADV_SEARCH;
    $search = $searchContact;
    include_once("search/save_search.inc");
}
elseif ($continue == 'second_contact_form_done')
{
	if (isset($searchContact) && is_string($searchContact))
	{
        $searchContact = new SearchContact($searchContact);
	}

	$searchContact->set_contact_values_2($search_company_name, $search_company_website_as, $search_company_website, $search_company_area
        , $search_company_country, $search_company_provstate, $search_company_region, $search_company_city, $search_company_postal_code
        , $search_company_phone_as, $search_company_phone, $search_company_fax_as, $search_company_fax, $search_company_email_as
        , $search_company_email, $search_company_yes_industry, $search_company_no_industry, $search_company_size, $search_company_type
        , $search_company_flags_boolean, $search_company_flags, $search_department_name, $search_department_website_as
        , $search_department_website, $search_department_area, $search_department_country, $search_department_provstate
        , $search_department_region, $search_department_city, $search_department_postal_code, $search_department_phone_as
        , $search_department_phone, $search_department_fax_as, $search_department_fax, $search_department_email_as
        , $search_department_email, $search_department_yes_industry, $search_department_no_industry, $search_department_size
        , $search_comp_or_div, $search_department_flags_boolean, $search_department_flags
        );

    $error_array = $searchContact->checkInputs();
    
    if (sizeof($error_array) > 0)
    {
		include('contact/view/contact/view_company_criteria.inc');
    }

	else
	{
        if($btnSearch=="Save Search")
        {
            //if the user wishes to save or edit the search
            $search_saved = true;
            $module_id = CONTACT_ADV_SEARCH;
            if (!is_string($searchContact))
            {
                $search = packObject($searchContact);
            }
            if($edit_page)
            {
                $continue = "Edit Search Criteria";
            }
            include_once("search/save_search.inc");
        }
        else
        {
            include('contact/view/contact/view_contact_search_results.inc');
        }
    }
}
elseif ($continue == 'Back to List' || $continue == 're-sort')
{
	if (isset($searchContact) && is_string($searchContact))
	{
        $searchContact = new SearchContact($searchContact);
	}

	include('contact/view/contact/view_contact_search_results.inc');
}

elseif ($continue == 'set_contact_search_results_mail')
{
    include('contact/view/contact/mail_contact_search_results.inc');
}

elseif ($continue == 'send_contact_search_results_mail')
{
    include('contact/view/contact/mail_contact_search_results.inc');

    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = new SearchContact($searchContact);
    }

    include('contact/view/contact/view_contact_search_results.inc');
}

elseif ($continue == 'View')
{
    if($search_id)
    {
        $sql = "Select search
            FROM search
            WHERE search_id = '$search_id'
            ";
        $results = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();
        $searchContact  = $row["search"];
        unset($search_id);
    }
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = new SearchContact($searchContact);
    }

    include('contact/view/contact/view_contact_search_results.inc');
}
elseif ($continue == 'Go to Simple Search')
{
    include('contact/view/contact/view_contact_criteria_simple.inc');
}
elseif ($continue == 'Go to Advanced Search')
{
    if($searchContact && is_string($searchContact) && $btnSearch="Edit Search Criteria")
    {
        //this part of the code is used when a search is loaded in a form for editing
        //here we are extracting the variable from the search object.
        $searchContact = new SearchContact($searchContact);
        $search_first_name = $searchContact->search_contact_first_name;
        $search_last_name = $searchContact->search_contact_last_name;
        $search_email_as = $searchContact->search_contact_email_as;
        $search_email = $searchContact->search_contact_email;
        $search_phone_as = $searchContact->search_contact_phone_as;
        $search_phone = $searchContact->search_contact_phone;
        $search_cellphone_as = $searchContact->search_contact_cellphone_as;
        $search_cellphone = $searchContact->search_contact_cellphone;
        $search_fax_as = $searchContact->search_contact_fax_as;
        $search_fax = $searchContact->search_contact_fax;
        $search_company_name = $searchContact->search_company_name;
        $search_division_name = $searchContact->search_department_name;
        $search_area = $searchContact->search_contact_area;
        $search_country = $searchContact->search_contact_country;
        $search_provstate = $searchContact->search_contact_provstate;
        $search_region = $searchContact->search_contact_region;
        $search_city = $searchContact->search_contact_city;
        $search_postal_code = $searchContact->search_contact_postal_code;
        $search_action = $searchContact->search_contact_action;
        $search_action_by = $searchContact->search_contact_action_by;
        $search_action_as = $searchContact->search_contact_action_as;
        $search_action_on = $searchContact->search_contact_action_on;
        $search_action_between1 = $searchContact->search_contact_action_between1;
        $search_action_between2 = $searchContact->search_contact_action_between2;
        $search_action_bf_as = $searchContact->search_contact_action_bf_as;
        $search_action_bf_on = $searchContact->search_contact_action_bf_on;
        $search_action_bf_between1 = $searchContact->search_contact_action_bf_between1;
        $search_action_bf_between2 = $searchContact->search_contact_action_bf_between2;
        $search_flags_boolean = $searchContact->search_contact_flags_boolean;
        $checked_flags = $searchContact->search_contact_flags;
        $checked_nflags = $searchContact->search_contact_nflags;
        $search_department_name = $searchContact->search_contact_department_name;
        $search_contact_status_status = $searchContact->search_contact_status_status;
        $search_contact_status_department = $searchContact->search_contact_status_department;

        $column = $searchContact->search_display;
        $order = $searchContact->search_order;

        $search_company_criteria = $searchContact->search_company_criteria;
        $search_department_criteria = $searchContact->search_department_criteria;
    }

    include('contact/view/contact/view_contact_criteria.inc');
}
elseif ($continue == 'company_quickview')
{
    $searchContact = new SearchContact('');
    $searchContact->search_on_employer_id($employer_id);
	include('contact/view/contact/view_contact_search_results.inc');
}
elseif ($continue == 'department_quickview')
{
	$searchContact = new SearchContact('');
    $searchContact->search_on_division_id($department_id);
	
	include('contact/view/contact/view_contact_search_results.inc');
}
elseif ($continue == 'view_specific_contact')
{
	if (isset($searchContact) && is_string($searchContact))
	{	
        $searchContact = new SearchContact($searchContact);
	}

	if ($contact_id)
	{
		// They've gotten results from their search, and now want to see a specific contact's information.
		$contact = new EditContact('');
		$contact->loadContactValues($contact_id);
	
		include('contact/view/contact/view_specific_contact.inc');
	}
	else
	{
		error("You must select a contact to view specific details.");
		include('contact/view/contact/view_contact_search_results.inc');
	}
}

elseif ($continue == 'set_contact_mail')
{
    include('contact/view/contact/mail_contact.inc');
}

elseif ($continue == 'send_contact_mail')
{
    include('contact/view/contact/mail_contact.inc');
    
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = new SearchContact($searchContact);
    }

    if ($contact_id)
    {
        // They've gotten results from their search, and now want to see a specific contact's information.
        $contact = new EditContact('');
        $contact->loadContactValues($contact_id);

        include('contact/view/contact/view_specific_contact.inc');
    }
    else
    {
        error("You must select a contact to view specific details.");
        include('contact/view/contact/view_contact_search_results.inc');
    }

}

elseif ($continue == 'E-mail all Matches')
{
	// They would like to e-mail all of the contact's that have been returned.

	if (isset($searchContact) && is_string($searchContact))
	{
        $searchContact = new SearchContact($searchContact);
	}

	include('contact/view/contact/email_all_matches.inc');
}
elseif ($continue == 'send_email_to_matches')
{
	// They've completed the e-mail form.
	
	include('contact/view/contact/email_all_matches.inc');
	
	// Now return them to the initial search screen.

	if (isset($searchContact) && is_string($searchContact))
	{
        $searchContact = new SearchContact($searchContact);
	}

	include('contact/view/contact/view_contact_search_results.inc');
}

?>
