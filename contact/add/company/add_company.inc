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
 | Filename: add_company.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This is the file that does all of the work involved with        |
 | adding a company.  It decides whether or not the user has filled out a form  |
 | yet, and if they haven't it displays the form.  If they have filled out the  |
 | form, then it checks to make sure that all of the information is valid and   |
 | then shows the user their inputs and prompts for confirmation.               |
 +------------------------------------------------------------------------------+

*/


$error = 0;

if (!$continue)
{
	// No form has been submitted yet, so display it for the user.
	include('contact/add/company/add_company_form.inc');
}

elseif ($continue == 'Continue')
{
	// Fix the address to be on subsequent lines.
	if (trim($company_street_address_1) == '' && trim($company_street_address_3) != '')
	{
		$company_street_address_2 = trim($company_street_address_3);
		$company_street_address_3 = '';
	}
	if (trim($company_street_address_1) == '' && trim($company_street_address_2) != '')
	{
		$company_street_address_1 = trim($company_street_address_2);
		$company_street_address_2 = '';
	}
	if (trim($company_street_address_1) == '' && trim($company_street_address_3) != '')
    {
            $company_street_address_2 = trim($company_street_address_3);
            $company_street_address_3 = '';
    }

    //Change "s to 's in the name
    $company_name = preg_replace("/\"/", "'", $company_name);

	// Trim all of the empty elements out of $company_flags.
	$company_flags = trim_array($company_flags);

	// Save the inputs in $contact.
	$contact = new AddContact('');
	$contact->set_company_values($company_name, $company_website, $company_description, $company_street_address_1,
        $company_street_address_2, $company_street_address_3, $company_city, $company_region_id, $company_provstate_id, 
	$company_country_id, $company_postal_code, $company_phone, $company_fax, $company_email, $company_flags, 
	$company_industry_id, $company_size_id, $company_new_note, $company_department_comments, $company_status, 
	$company_type_id);


	// Check that all inputs are valid.
	if (trim($contact->company_name) == '')
	{
		$error = 1;
		error("To add a company, you must include a company name.");
		include('contact/add/company/add_company_form.inc');
	}
	elseif ((trim($contact->company_email) != '') && !(isValidEmail($contact->company_email)))
	{
		$error = 1;
		error("You must include a valid set of e-mail addresses, or none at all.");
		include('contact/add/company/add_company_form.inc');
	}
	elseif ((trim($contact->company_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->company_phone)))))
        {
                $error = 1;
                error("You must include a valid phone number with an area code, or none at all.");
                include('contact/add/company/add_company_form.inc');
        }
        elseif ((trim($contact->company_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact->company_fax)))))
        {
                $error = 1;
                error("You must include a valid fax number with an area code, or none at all.");
                include('contact/add/company/add_company_form.inc');
        }
	elseif (!trim($contact->company_type_id))
	{
		$error = 1;
		error("You must choose a company type for the company you are adding.");
		include('contact/add/company/add_company_form.inc');
	}
	elseif (!trim($contact->company_industry_id))
	{
		$error = 1;
		error("You must choose an industry for the company you are adding.");
		include('contact/add/company/add_company_form.inc');
	} 
    /*
     :TODO: Test that this code isn't needed, and then remove.
     - Company statuses are automatically set as CONTACT_POTENTIAL_LEAD upon entry now.
	elseif (!trim($contact->company_status))
	{
		$error = 1;
		error("You must choose a status for the company you are adding.");
		include('contact/add/company/add_company_form.inc');
	}
    */

	if ($error == 0)
	{
		include('contact/add/company/confirm_company_inputs.inc');	
	}

}

elseif ($continue == 'add_confirmed')
{
	if ($add_confirmed && is_string($contact))
	{
		$contact = unpackObject($contact);
	}
	include('contact/add/company/confirm_company_inputs.inc');
}

elseif ($continue == 'Add Company Now')
{
	$contact = unpackObject($contact);
	include('contact/add/company/save_company.inc');
}

elseif ($continue == 'Edit Company')
{
	$contact = unpackObject($contact);
	include('contact/add/company/add_company_form.inc');
}
	
?>
