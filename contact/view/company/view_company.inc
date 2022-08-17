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
 | Filename: view_company.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file does all of the grunt work.  It deteremines which     |
 | form needs to be loaded for the user, and loads that form appropriately.     |
 +------------------------------------------------------------------------------+

*/

//:TODO Show ALL of the fields, regardless of whether or not they have been set.

include_once('contact/view/SearchContact.class');
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
        include ('contact/view/company/view_company_criteria_simple.inc');
    }
    else if (($array = $response->FetchRow()) && $array['search_use_advanced_emp'] == 1)
    {
        include ('contact/view/company/view_company_criteria.inc');
    }
    else
    {
        include ('contact/view/company/view_company_criteria_simple.inc');
    }
}                       

elseif ($continue == 'first_company_form_done')
{                       
    $error = 0;

    /*
     Validate the users inputs before searching.  The only thing we need to ensure
     is that they've chosen some kind of outputs to display on the screen, and that
     any dates they have entered are correctly formatted.
     */

    $display1 = 'name';
    $valid_input = 0;
    for ($i = 1; $i < 7; $i++)
    {
        $varname = ("display" . $i);
        $variable = $$varname;

        if (($variable) && (trim($variable) != 'ignore'))
        {
            $valid_input = true;
            break;
        }
    }

    if (!$valid_input)
    {
        $error = 1;
        error("You must specify at least one field to be displayed in your search results.");
        include('contact/view/company/view_company_critieria.inc');
    }

    if ($error == 0)
    {

        if (trim($search_name_m))
        {
            $search_name = $search_name_m;
        }

        // Reploace "s with 's in the name.
        $search_name = preg_replace("/\"/", "'", $search_name);

        $searchContact = new SearchContact('');


        $searchContact->set_company_values_1(
                $search_name, $search_name_as, $search_website_as, $search_website, $search_area, $search_country, $search_provstate, $search_region,
                $search_city, $search_postal_code, $search_phone_as, $search_phone, $search_email_as, $search_email, $yes_industries, 
                $no_industries, $search_size, $search_type, $display1, $display2, $display3, $display4, $display5, $display6, $display7, $display8,
                $order1, $order2, $order3, $order4, $search_comp_or_div, $search_flags_boolean, $checked_flags, $search_company_status_status,
                $search_company_status_department
                );
        if($btnSearch=="Save Search")
        {
            //if the user wishes to save or edit the search
            $module_id = COMPANY_DIVISION_ADV_SEARCH;
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
            include('contact/view/company/view_company_search_results.inc');
        }
    }
}
elseif($continue == 'save_company_search')
{
    $module_id = COMPANY_DIVISION_ADV_SEARCH;
    $search = $searchContact;
    include_once("search/save_search.inc");
}
elseif ($continue == 'view_company_search_results')
{
    if($search_id)
    {
        $search_saved = true;
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
       $searchContact = unpackObject($searchContact); 
    }

    include('contact/view/company/view_company_search_results.inc');
}

elseif ($continue == 'view_division_search_results')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    include('contact/view/company/view_department_search_results.inc');
}

elseif ($continue == 'Back to List')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    include('contact/view/company/view_company_search_results.inc');
}

elseif ($continue == 'back_to_department_list')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    include('contact/view/company/view_department_search_results.inc');
}

elseif ($continue == 'set_company_search_results_mail')
{
    include('contact/view/company/mail_company_search_results.inc');
}

elseif ($continue == 'send_company_search_results_mail')
{
    include('contact/view/company/mail_company_search_results.inc');
    
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchObject);
    }

    include('contact/view/company/view_company_search_results.inc');
}

elseif ($continue == 'set_department_search_results_mail')
{
    include('contact/view/company/mail_department_search_results.inc');
}

elseif ($continue == 'send_department_search_results_mail')
{
    include('contact/view/company/mail_department_search_results.inc');

    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchObject);
    }

    include('contact/view/company/view_department_search_results.inc');
}

elseif ($continue == 'view_specific_details')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchObject);
    }

    if ($department_id)
    {
        // They've gotten the results from their search, and now want to see a specific divisions info.
        $contact = new EditContact('');
        $contact->loadDepartmentValues($department_id);

        include('contact/view/company/view_specific_department.inc');
    }
    elseif ($employer_id)
    {
        // They've gotten results from their search, and now want to see a specific contact's information.
        $contact = new EditContact('');
        $contact->loadCompanyValues($employer_id);
        include('contact/view/company/view_specific_company.inc');
    }
    else
    {
        error("You must select a company or division to view specific details.");
        include('contact/view/company/view_company_search_results.inc');
    }
}

// Begin of new code.
elseif ($continue == 'set_mail')
{
    include('contact/view/company/mail_company.inc');
}

elseif ($continue == 'send_mail')
{
    include('contact/view/company/mail_company.inc');
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    if ($department_id)
    {
        // They've gotten the results from their search, and now want to see a specific divisions info.
        $contact = new EditContact('');
        $contact->loadDepartmentValues($department_id);

        include('contact/view/company/view_specific_department.inc');
    }
    elseif ($employer_id)
    {
        // They've gotten results from their search, and now want to see a specific contact's information.

        $contact = new EditContact('');
        $contact->loadCompanyValues($employer_id);
        include('contact/view/company/view_specific_company.inc');
    }
    else
    {
        error("You must select a company or division to view specific details.");
        include('contact/view/company/view_company_search_results.inc');
    }

}
// End of new code.


elseif ($continue == 'View')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }
    if ($show_more == 'departments')
    {
        include('contact/view/company/view_department_search_results.inc');
    }
    else
    {
        include('contact/view/company/view_company_search_results.inc');
    }
}

elseif ($continue == 'view_specific_company')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    if ($employer_id)
    {
        // They've gotten results from their search, and now want to see a specific company's information.

        $contact = new EditContact('');
        $contact->loadCompanyValues($employer_id);

        include('contact/view/company/view_specific_company.inc');
    }
    else
    {
        error("You must select a company to view specific details.");
        include('contact/view/company/view_company_search_results.inc');
    }
}

elseif ($continue == 'view_specific_department')
{
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    if ($department_id)
    {
        // They've gotten results from their search, and now want to see a specific contact's information.

        $contact = new EditContact('');
        $contact->loadDepartmentValues($department_id);

        include('contact/view/company/view_specific_department.inc');
    }
    else
    {
        error("You must select a division to view specific details.");
        include('contact/view/company/view_department_search_results.inc');
    }
}
elseif ($continue == 'set_mail_department')
{
    include('contact/view/company/mail_department.inc');
}
elseif ($continue == 'send_mail_department')
{
    include('contact/view/company/mail_department.inc');
    if (isset($searchContact) && is_string($searchContact))
    {
        $searchContact = unpackObject($searchContact);
    }

    if ($department_id)
    {
        // They've gotten results from their search, and now want to see a specific contact's information.

        $contact = new EditContact('');
        $contact->loadDepartmentValues($department_id);

        include('contact/view/company/view_specific_department.inc');
    }
    else
    {
        error("You must select a division to view specific details.");
        include('contact/view/company/view_department_search_results.inc');
    }
}
elseif ($continue == 'Go to Simple Search')
{
    include('contact/view/company/view_company_criteria_simple.inc');
}
elseif ($continue == 'Go to Advanced Search')
{
    //this part of the code is used when a search is loaded in a form for editing
    //here we are extracting the variable from the search object.
    if($searchContact && $btnSearch="Edit Search Criteria")
    {

        $searchContact = unpackObject($searchContact);
        $search_name_m = $searchContact->search_company_name;
        $search_name_as = $searchContact->search_company_name_as;
        $search_website_as = $searchContact->search_company_website_as;
        $search_website = $searchContact->search_company_website;
        $search_area = $searchContact->search_company_area;
        $search_country = $searchContact->search_company_country;
        $search_provstate = $searchContact->search_company_provstate;
        $search_region = $searchContact->search_company_region;
        $search_city = $searchContact->search_company_city;
        $search_postal_code = $searchContact->search_company_postal_code;
        $search_phone_as = $searchContact->search_company_phone_as;
        $search_phone = $searchContact->search_company_phone;
        $search_name = $searchContact->search_company_name;
        $search_email_as = $searchContact->search_company_email_as;
        $search_email = $searchContact->search_company_email;
        $yes_industries = $searchContact->search_company_yes_industry;
        $no_industries = $searchContact->search_company_no_industry;
        $search_size = $searchContact->search_company_size;
        $search_type = $searchContact->search_company_type;
        $search_flags_boolean = $searchContact->search_company_flags_boolean;
        $search_flags = $searchContact->search_company_flags;
        $search_company_status_status = $searchContact->search_company_status_status;
        $search_company_status_department = $searchContact->search_company_status_department;

        $display1 = $searchContact->search_display1;
        $display2 = $searchContact->search_display2;
        $display3 = $searchContact->search_display3;
        $display4 = $searchContact->search_display4;
        $display5 = $searchContact->search_display5;
        $display6 = $searchContact->search_display6;
        $display7 = $searchContact->search_display7;
        $display8 = $searchContact->search_display8;
        $order1 = $searchContact->search_order1;
        $order2 = $searchContact->search_order2;
        $order3 = $searchContact->search_order3;
        $order4 = $searchContact->search_order4;

        $search_comp_or_div = $searchContact->search_comp_or_div;

    }
    include('contact/view/company/view_company_criteria.inc');
}
elseif ($continue == 'company_quickview')
{
    $searchContact = new SearchContact('');
    $searchContact->search_on_employer_id($employer_id);

    $searchContact->search_display1 = 'name';
    $searchContact->search_display2 = 'phone';
    $searchContact->search_display3 = 'email';
    $searchContact->search_display4 = 'company';

    include('contact/view/company/view_department_search_results.inc');
}

?>
