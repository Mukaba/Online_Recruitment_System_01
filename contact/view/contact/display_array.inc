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
 | Filename: display_array.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This controls the select and order by statements in the         |
 | contact advanced search's query. This also populates the display column      |
 | and order column drop down menus at the initial advanced search page. This   |
 | also controls the 'header' of columns displayed in the advanced search       |
 | results.                                                                     |
 +------------------------------------------------------------------------------+

*/

// ORDER array elements with a -- prefix will need to be run before the ORDER switch statement in the contact results page. It will check
// the value array to make sure that the CASE statement has not been added to the SQL query first. If it has, don't use it. 

// This is because if someone just ORDER's a street address per say, the ORDER BY clause is c_street_address_1. This doesn't work because
// the alias hasn't been built yet. 

$display_array[] = array(
    "name" => "&nbsp;",
    "select" => "",
    "alias" => "",
    "order" => "",
    "select_required" => ""
    );
$display_array[] = array(
    "name" => "Contact Address",
    "select" => ("
        IF (c.location_info = '".USE_COMPANY."'
           , ec.street_address_1
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ec.street_address_1
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , ed.street_address_1
                     , c.street_address_1
                  )
             )
        )
        AS c_street_address_1,

        IF (c.location_info = '".USE_COMPANY."'
             , ec.street_address_2
             , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ec.street_address_2
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , ed.street_address_2
                     , c.street_address_2)))
        AS c_street_address_2,
        
        IF (c.location_info = '".USE_COMPANY."'
           , ec.street_address_3
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ec.street_address_3
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , ed.street_address_3
                     , c.street_address_3)))
        AS c_street_address_3
        "),
    "alias" => array("c_street_address_1","c_street_address_2","c_street_address_3"),
    "order" => "c_street_address_1",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Cell Number",
    "select" => "c.cellphone AS c_cellphone",
    "alias" => "c_cellphone",
    "order" => "c.cellphone",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact City",
    "select" => ("
        IF (c.location_info = '".USE_COMPANY."'
           , ec.city
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ec.city
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , ed.city
                     , c.city)))
        AS c_city
        "),
    "alias" => "c_city",
    "order" => "c_city",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Country",
    "select" => ("
        IF (c.location_info = '".USE_COMPANY."'
           , eccl.country_name
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , eccl.country_name
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , edcl.country_name
                     , ccl.country_name)))
        AS c_country_name
        "),
    "alias" => "c_country_name",
    "order" => "c_country_name",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Department Name",
    "select" => "ce.department_name AS ce_department_name",
    "alias" => "ce_department_name",
    "order" => "ce.department_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact E-mail",
    "select" => "c.email AS c_email",
    "alias" => "c_email",
    "order" => "c.email",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Fax Number",
    "select" => ("
        IF (c.fax = '".USE_COMPANY."'
           , ec.fax
           , IF (c.fax = '".USE_DEPARTMENT."' AND ed.fax = '".USE_COMPANY."'
                , ec.fax
                , IF (c.fax = '".USE_DEPARTMENT."'
                     , ed.fax
                     , c.fax)))
        AS c_fax
        "),
    "alias" => "c_fax",
    "order" => "c_fax",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact First Name",
    "select" => "c.first_name AS c_first_name",
    "alias" => "c_first_name",
    "order" => "c.first_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Last Name",
    "select" => "c.last_name AS c_last_name",
    "alias" => "c_last_name",
    "order" => "c.last_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Pager",
    "select" => "c.pager AS c_pager",
    "alias" => "c_pager",
    "order" => "c.pager",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Phone Number",
    "select" => ("
        IF (c.phone = '".USE_COMPANY."'
           , ec.phone
           , IF (c.phone = '".USE_DEPARTMENT."' AND ed.phone = '".USE_COMPANY."'
                , ec.phone
                , IF (c.phone = '".USE_DEPARTMENT."'
                     , ed.phone
                     , c.phone)))
        AS c_phone
        "),
    "alias" => "c_phone",
    "order" => "c_phone",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Position",
    "select" => "ce.position AS c_position",
    "alias" => "c_position",
    "order" => "ce.position",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Postal Code",
    "select" => ("
        IF (c.location_info = '".USE_COMPANY."'
           , ec.postal_code
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ec.postal_code
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , ed.postal_code
                     , c.postal_code)))
        AS c_postal_code
        "),
    "alias" => "c_postal_code",
    "order" => "c_postal_code",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Province",
    "select" => ("
        IF (c.location_info = '".USE_COMPANY."'
           , ecpl.provstate_name
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ecpl.provstate_name
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , edpl.provstate_name
                     , cpl.provstate_name)))
        AS c_provstate_name
        "),
    "alias" => "c_provstate_name",
    "order" => "c_provstate_name",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Region",
    "select" => ("
        IF (c.location_info = '".USE_COMPANY."'
           , ecrl.region_name
           , IF (c.location_info = '".USE_DEPARTMENT."' AND ed.location_info = '".USE_COMPANY."'
                , ecrl.region_name
                , IF (c.location_info = '".USE_DEPARTMENT."'
                     , edrl.region_name
                     , crl.region_name)))
        AS c_region_name
        "),
    "alias" => "c_region_name",
    "order" => "c_region_name",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Contact Status",
    "select" => "ceis.status_name AS c_status",
    "alias" => "c_status",
    "order" => "ceis.level_of_activity",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Title",
    "select" => "ctitle.title_name AS c_title",
    "alias" => "c_title",
    "order" => "ctitle.title_name",
    "select_required" => "0"
    );

// Begin division cases

$display_array[] = array(
    "name" => "Division Address",
    "select" => ("
        IF (ed.location_info = '".USE_COMPANY."'
           , ec.street_address_1
           , ed.street_address_1)
        AS ed_street_address_1,
                
        IF (ed.location_info = '".USE_COMPANY."'
             , ec.street_address_2
             , ed.street_address_2)
        AS ed_street_address_2,
                
        IF (ed.location_info = '".USE_COMPANY."'
             , ec.street_address_3
             , ed.street_address_3)
        AS ed_street_address_3
        "),
    "alias" => array("ed_street_address_1","ed_street_address_2","ed_street_address_3"),
    "order" => "ed_street_address_1",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Division City",
    "select" => ("
        IF (ed.location_info = '".USE_COMPANY."'
           , ec.city
           , ed.city)
        AS ed_city
        "),
    "alias" => "ed_city",
    "order" => "ed_city",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Division Country",
    "select" => ("
        IF (ed.location_info = '".USE_COMPANY."'
           , eccl.country_name
           , edcl.country_name)
        AS ed_country_name
        "),
    "alias" => "ed_country_name",
    "order" => "ed_country_name",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Division E-mail",
    "select" => "ed.email AS ed_email",
    "alias" => "ed_email",
    "order" => "ed.email",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Division Fax Number",
    "select" => ("
        IF (ed.fax = '".USE_COMPANY."'
           , ec.fax
           , ed.fax)
        AS ed_fax
        "),
    "alias" => "ed_fax",
    "order" => "ed_fax",
    "select_required" => "1"
    );
$display_array[] = array( // employer_department's industry
    "name" => "Division Industry",
    "select" => "edind.industry_name AS ed_industry_name",
    "alias" => "ed_industry_name",
    "order" => "edind.industry_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Division Name",
    "select" => "ed.department_name AS ed_department_name",
    "alias" => "ed_department_name",
    "order" => "ed.department_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Division Phone Number",
    "select" => ("
        IF (ed.phone = '".USE_COMPANY."'
           , ec.phone
           , ed.phone)
        AS ed_phone
        "),
    "alias" => "ed_phone",
    "order" => "ed_phone",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Division Postal Code",
    "select" => ("
        IF (ed.location_info = '".USE_COMPANY."'
           , ec.postal_code
           , ed.postal_code)
        AS ed_postal_code
        "),
    "alias" => "ed_postal_code",
    "order" => "ed_postal_code",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Division Province",
    "select" => ("
        IF (ed.location_info = '".USE_COMPANY."'
           , ecpl.provstate_name
           , edpl.provstate_name)
        AS ed_provstate_name
        "),
    "alias" => "ed_provstate_name",
    "order" => "ed_provstate_name",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Division Region",
    "select" => ("
        IF (ed.location_info = '".USE_COMPANY."'
           , ecrl.region_name
           , edrl.region_name)
        AS ed_region_name
        "),
    "alias" => "ed_region_name",
    "order" => "ed_region_name",
    "select_required" => "1"
    );
$display_array[] = array( // employer_department's employer size table. 
    "name" => "Division Size",
    "select" => "edes.size_range AS ed_size_range",
    "alias" => "ed_size_range",
    "order" => "edes.size_range",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Division Website",
    "select" => "ed.department_website AS ed_website",
    "alias" => "ed_website",
    "order" => "ed.department_website",
    "select_required" => "0"
    );

// Begin company cases

$display_array[] = array(
    "name" => "Company Address",
    "select" => "ec.street_address_1 AS ec_street_address_1,ec.street_address_2 AS ec_street_address_2,ec.street_address_3 AS ec_street_address_3",
    "alias" => array("ec_street_address_1","ec_street_address_2","ec_street_address_3"),
    "order" => "ec_street_address_1",
    "select_required" => "1"
    );
$display_array[] = array(
    "name" => "Company City",
    "select" => "ec.city AS ec_city",
    "alias" => "ec_city",
    "order" => "ec.city",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Country",
    "select" => "eccl.country_name AS ec_country_name",
    "alias" => "ec_country_name",
    "order" => "eccl.country_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company E-mail",
    "select" => "ec.email AS ec_email",
    "alias" => "ec_email",
    "order" => "ec.email",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Fax Number",
    "select" => "ec.fax AS ec_fax",
    "alias" => "ec_fax",
    "order" => "ec.fax",
    "select_required" => "0"
    );
$display_array[] = array( // employer_company's industry
    "name" => "Company Industry",
    "select" => "ecind.industry_name AS ec_industry_name",
    "alias" => "ec_industry_name",
    "order" => "ecind.industry_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Name",
    "select" => "ec.company_name AS ec_company_name",
    "alias" => "ec_company_name",
    "order" => "ec.company_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Name, Division Name",
    "select" => "ec.company_name as ec_company_name, ed.department_name AS ed_department_name",
    "alias" => "company_division_name",
    "order" => "ec_company_name, ed_department_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Phone Number",
    "select" => "ec.phone AS ec_phone",
    "alias" => "ec_phone",
    "order" => "ec.phone",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Postal Code",
    "select" => "ec.postal_code AS ec_postal_code",
    "alias" => "ec_postal_code",
    "order" => "ec.postal_code",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Province",
    "select" => "ecpl.provstate_name AS ec_provstate_name",
    "alias" => "ec_provstate_name",
    "order" => "ecpl.provstate_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Region",
    "select" => "ecrl.region_name AS ec_region_name",
    "alias" => "ec_region_name",
    "order" => "ecrl.region_name",
    "select_required" => "0"
    );
$display_array[] = array( // employer_company's employer size table. 
    "name" => "Company Size",
    "select" => "eces.size_range AS ec_size_range",
    "alias" => "ec_size_range",
    "order" => "eces.size_range",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Type",
    "select" => "ecct.type_name AS ec_company_type",
    "alias" => "ec_company_type",
    "order" => "ecct.type_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Company Website",
    "select" => "ec.website AS ec_website",
    "alias" => "ec_website",
    "order" => "ec.website",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Action Type",
    "select" => "cat.brief_action_name",
    "alias" => "brief_action_name",
    "order" => "brief_action_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Action By",
    "select" => "cab_c.last_name AS cab_c_last_name, cab_c.first_name AS cab_c_first_name",
    "alias" => "cab_c_name",
    "order" => "cab_c.last_name, cab_c.first_name",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Action On",
    "select" => "IF (cas.action_on = '0000-00-00', NULL, cas.action_on) AS cas_action_on",
    "alias" => "cas_action_on",
    "order" => "cas.action_on",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Action BF Date",
    "select" => "IF (cas.bring_forward_date = '0000-00-00', NULL, cas.bring_forward_date) AS cas_bring_forward_date",
    "alias" => "cas_bring_forward_date",
    "order" => "cas.bring_forward_date",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Action Method",
    "select" => "cam.action_method_name AS cam_action_method_name",
    "alias" => "cam_action_method_name",
    "order" => "cam.action_method_name",
    "select_required" => "0"
    );
// we can't really order by contact flags because it's a list of flags that are comma separated
$display_array[] = array(
    "name" => "Contact Flags",
    "select" => "",
    "alias" => "contact_flags",
    "order" => "",
    "select_required" => "0"
    );
$display_array[] = array(
    "name" => "Contact Status In My Dept",
    "select" => "my_ceis.status_name AS my_c_status",
    "alias" => "my_c_status",
    "order" => "my_ceis.level_of_activity",
    "select_required" => "0"
    );
?>
