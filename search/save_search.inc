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
 | Filename: save_search.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is used to save searched and its description          |
 +------------------------------------------------------------------------------+

*/

if($continue == "Save" || $continue == "Save and Search")
{
    //Over here we are check ing if the search name is added
    //and if it is added it doesnt conflict with some other search name
    $search_name = trim($search_name); 
    if(!$search_name || $search_name == "")
    {
        $msg = "Please enter a valid search name";
    }
    else
    {   
        if($search_id)
        {
            $sql = "SELECT s.search_name, sm.module_name
                    FROM search as s, search_module as sm
                    WHERE s.contact_id = '".$auth->contact_id."'
                    AND s.search_module_id = sm.module_id
                    AND s.search_name = '".$search_name."'
                    AND sm.module_id = '".$module_id."'";
            
            $result = $GLOBALS['dbh']->Execute($sql);
            if( $result->RecordCount() > 0)
            {
                $row = $result->FetchRow();
                $module_name = $row["module_name"];

                $msg = "The search name '$search_name' already exists for $module_name";
            }
        }
    }
    
    if($msg)                //if an error occurs display the error and reload the page
    {
        switch($module_id)  //getting the headers
        {
            case STUDENT_ADV_SEARCH:       //Student advanced search header
                $continue = "save search";
                include('student_info/view/view_student.inc');
                break;

            case HISTORY_ADV_SEARCH:       //History advanced search header
                $continue = "save search";                
?>

                </td>
                <td align="left">
                    <img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>">
                </td>
                </tr>
                </table>
                
<?php
                echo("<br /><br />");
                break;

            case COMPANY_DIVISION_ADV_SEARCH:       //Company advanced search header
                $level1 = "company";
                include("contact/view/view.inc");
                break;

            case CONTACT_ADV_SEARCH:       //Contact advanced search header
                $level1 = "contact";
                include("contact/view/view.inc");
                break;
        }
        
        error($msg);
        unset($msg);
        include_once('search/save_search_form.inc');
    }   
    else               //otherwise if there was no problem save the search and display the results
    {
        save_search($search, $search_name, $auth->contact_id, $module_id, $search_description);
        switch($module_id)
        {
            case STUDENT_ADV_SEARCH:  //Student advanced search save
                $btnSearch="Search";
                $searchStudents = $search;
                $continue = 'Advanced Search';
                include('student_info/view/view_student.inc');
                break;

            case HISTORY_ADV_SEARCH:  //History advanced search save
                $btnSearch="Search";
                $searchHistory = $search;
                include('history/advanced_search_form.inc');
                break;

            case COMPANY_DIVISION_ADV_SEARCH:  //Company advanced search save
                $btnSearch="Search";
                $level1 = "company";
                $continue = 'view_company_search_results';
                $searchContact = $search;
                include("contact/view/view.inc");
                break;

            case CONTACT_ADV_SEARCH:  //Contact advanced search save
                $btnSearch="Search";
                $level1 = "contact";
                $continue = 'View';
                $searchContact = $search;
                include("contact/view/view.inc");
                break;
        }
    }
}
else
{    
    include_once('search/save_search_form.inc');  //Display the form where you can name and describe the search
}

//this function saves the searches.
function save_search($search, $search_name, $contact_id, $module_id, $search_description)
{
    $search_name = strip_tags($search_name,'');
    if($search_description)
    {
        $search_description = strip_tags($search_description,'');
    }
    
    $sql = ("INSERT INTO search
             (search, search_name, contact_id, search_module_id, search_description)
             VALUES
             ('" . $search . "',
              '" . $search_name . "',
              '" . $contact_id . "',
              '" . $module_id . "', 
              '" . $search_description . "')");

    $result = $GLOBALS['dbh']->Execute($sql);
}
?>
