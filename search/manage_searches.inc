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
 | Filename: manage_searches.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: You can edit search form, delete a search, perform a search     |
 | and edit the description and name of a search all from this file             |
 +------------------------------------------------------------------------------+

*/

unset($no_tables);
if($continue=="Perform Search")
{
    if($search_id)
    {
        //Here we are extracting the saved search from the database
        $sql = "SELECT search, search_module_id
                FROM search
                WHERE contact_id = '".$auth->contact_id."'
                AND search_id = $search_id";
                
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $search = $row["search"];
        $module_id = $row["search_module_id"];
        $search_saved = true;
        
        switch($module_id)  //The variables are set according to the type of Advanced Search (i.e. which module we came from)
        {
            case STUDENT_ADV_SEARCH:       //Student advanced search
                $btnSearch="Search";
                $searchStudents = $search;
                $continue = 'Advanced Search';
                include('student_info/view/view_student.inc');
                break;

            case HISTORY_ADV_SEARCH:       //History advanced search
                $btnSearch="Search";
                $searchHistory = $search;
                include('history/advanced_search_form.inc');
                break;

            case COMPANY_DIVISION_ADV_SEARCH:       //company advanced search
                $btnSearch="Search";
                $level1 = "company";
                $continue = 'view_company_search_results';
                $searchContact = $search;
                include("contact/view/view.inc");
                break;

            case CONTACT_ADV_SEARCH:       //contact advanced search
                $btnSearch="Search";
                $level1 = "contact";
                $continue = 'View';
                $searchContact = $search;
                include("contact/view/view.inc");
                break;
        }
        
        $no_tables = true;  //by setting "no tables" the search tables are not visible anymore
    }
    else                    //if a search is not selected the error is displayed
    {
        $emsg = "You must select a search";
    }
}
elseif($continue=="Edit Search Name/Description")  //Then we want to go to the save_search_form
{
    if($search_id)
    {
        include_once("search/save_search_form.inc");  //used to save the name and description of a search
        $no_tables = true;                            //by setting "no tables" the search tables are not visible anymore
    }
    else
    {
        $emsg = "You must select a search";
    }
}
//Edit Search2 is set when you are comming from editing search form
//After hitting "save search" in a search form you will now go to
//edit search description and name
elseif($continue=="Edit Search2")
{
    $continue="Edit Search Criteria";
    include_once("search/save_search_form.inc");
    $no_tables = true;                                //by setting "no tables" the search tables are not visible anymore
}
elseif($continue=="Edit Search Criteria")                 //then we want to go to the saved 
{
    if($search_id)
    { 
        //here we are getting the saved search from the db
        $sql = "SELECT search, search_module_id
                FROM search
                WHERE contact_id = '".$auth->contact_id."'
                AND search_id = $search_id";
                
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $search = $row["search"];
        $module_id = $row["search_module_id"];
        $search_saved = true;
        
        switch($module_id)  //The variables are set according to the type of Advanced Search (i.e. what module we came from)
        {
            case STUDENT_ADV_SEARCH:       //Student advanced search form
                $btnSearch="Edit Search Criteria";
                $searchStudents = $search;
                $continue = 'Advanced Search';
                include('student_info/view/view_student.inc');
                break;

            case HISTORY_ADV_SEARCH:       //History advanced search form
                $btnSearch="Edit Search Criteria";
                $searchHistory = $search;
                $continue = 'Advanced Search';
                include('history/advanced_search_form.inc');
                break;

            case COMPANY_DIVISION_ADV_SEARCH:       //Company advanced search form
                $btnSearch="Edit Search Criteria";
                $level1 = "company";
                $continue = 'Go to Advanced Search';
                $searchContact = $search;
                include("contact/view/view.inc");
                break;

            case CONTACT_ADV_SEARCH:       //Contact advanced search form
                $btnSearch="Edit Search Criteria";
                $level1 = "contact";
                $continue = 'Go to Advanced Search';
                $searchContact = $search;
                include("contact/view/view.inc");
                break;
        }
        
        $no_tables = true;   //by setting "no tables" the search tables are not visible anymore
    }
    else
    { 
        $emsg = "You must select a search";
    }
}
elseif($continue=="Delete Search")    //delete a selected search
{
    if($search_id)
    {
        //extracting the search name and the module name
        $sql = "SELECT s.search_name, sm.module_name
                FROM search as s, search_module as sm
                WHERE s.search_module_id = sm.module_id
                AND s.search_id = '".$search_id."'";
        $result = $GLOBALS['dbh']->Execute($sql);
        
        if($result->RecordCount() > 0)  //if there is a search with that id delete it
        {
?>

            </td>
            <td align="left">
                <img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" />
            </td>
            </tr>
            </table>
            
            <form action='<?php echo $PHP_SELF?>' method='post'>
            <input type='hidden' name='select' value='manage_searches' />

            <h3 align='left'>Manage Searches</h3>
            
<?php
            $row = $result->FetchRow();
            
            $module_name = $row["module_name"];
            $search_name = $row["search_name"];
            echo("<table border='0' width='100%' align='LEFT'>");
                echo("<tr class='row1'>");
                    echo("<td>");
                    
                    //give the user a warning before deleting
                    instruct("Are you sure you want delete <b class=instruct> '$search_name' </b> from <b class=instruct> $module_name ?</b><br /><br /><br />
                              <center>
                                  <input type='hidden' name='search_id' value='$search_id' />
                                  <input type='submit' name='continue' value='Yes' />&nbsp;
                                  <input type='submit' name='continue' value='No' />
                              </center>");
                    
                    echo("</td>");
                echo("</tr>");
            echo("</table>");
            echo("</form>");
            
            $no_tables = true;   //by setting "no tables" the search tables are not visible anymore
        }
        else
        {
            $emsg = "There was an error while processing your request";
        }    
    }
    else
    {
        $emsg = "You must select a search";
    }
}
elseif($continue=="Yes")  //If deleting a search is confirmed
{
    if($search_id)
    {
        $sql = ("DELETE FROM search
                 WHERE search_id = '$search_id'");
        
        $result = $GLOBALS['dbh']->Execute($sql);
        $nmsg="The selected search was deleted";
    }
    else
    {
        $emsg = "You must select a search";
    }

}
elseif(($continue == "Check Unique Name") || ($continue == "Update Approval") || ($continue == "Check Update"))  //Update a saved search
{
    $no_tables = true;                                    //turn off the list of saved searches
    
    if($continue=="Check Unique Name")                    //Let's check to see if this saved search name is unique
    {   
        if(!($search_name) || ($search_name == ""))       //checking that the user has entered a name for the search
        {
            $msg = "Please enter a valid search name";    //create the error message to pass to the save search form 
            $continue = "Edit Search Name/Description";                    

            include_once("search/save_search_form.inc");  //show them the save form again to give them a chance to re-enter a name

            unset($msg);                                  
        }
        else
        {
            //Determine whether or not the submitted search name is in the database
            $search_name_sql = ("
                SELECT s.search_name, s.search_id
                FROM search AS s
                WHERE s.search_name = '$search_name' AND s.contact_id = '".$auth->contact_id."' AND s.search_module_id = '".$module_id."'
                ");
            
            $search_name_result = $GLOBALS['dbh']->Execute($search_name_sql);
            
            $row = $search_name_result->FetchRow();
            $temp_search_id = $row["search_id"];

            // If we come into this part of code without a search_id, then we take one from the SQL query.
            if (!$search_id)
            {
                $search_id = $temp_search_id;
            }
             
            if($search_name_result->RecordCount() > 0)      //if this search_name already exists in the database
            {
                $search_id = $temp_search_id;
                $continue = "Update Approval";            //then let us attempt to find a solution to this problem
            }
            // If we get no matching search_id from the database, then we use the search_id that was passed into this form. 
            else                                          //otherwise this search_name is unique
            {
                $continue = "Save";
                include_once('search/save_search.inc');   //Display the form where you can name and describe the search
            }
        }
    }
    elseif($continue == "Check Update")                   //We must do error checking from the Update Confirmation page
    {
        //Check to see if this search already exists
        $search_name_sql = "SELECT s.search_name, s.search_id         
                            FROM search AS s
                            WHERE s.search_name = '$new_search_name'";

        $search_name_result = $GLOBALS['dbh']->Execute($search_name_sql);
        
        if((!($new_search_name) || ($new_search_name == "")) && ($search_update_type == "Rename Search"))  //if new name entered is null or an empty string
        {
            $continue = "Update Approval";                                                                 //reload the Update Search Confirmation page
            $msg = "Please enter a valid search name.";                                                    //and tell them the name is invalid.
        }
        elseif(($search_name_result->RecordCount() > 0) && ($search_update_type == "Rename Search"))         //if this name already exists in the database
        {
            $continue = "Update Approval";                                                                 //reload the Update Search Confirmation Page
            $msg = "The search name ".$new_search_name." already exists.  Please enter a unique name.";    //and tell them the name is not unique.
        }
        elseif($search_update_type == "Cancel")                                                            //the cancel button has been pressed 
        {
            unset($continue);                                                                              //then skip the user out of the update process
            $no_tables = false;                                                                            //display the list of saved searches
        }
        else                                                                                               //otherwise this name is unique
        {
            $continue = $search_update_type;                 //so set this variable to skip to the appropriate section in the code (either Rename or Update)
        }
    }

    if($continue == "Update Approval")                       //This section contains code that is used to confirm a search save update 
    {    
?>

        </td>
        <td align="left">
            <img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?> "alt="" />
        </td>
        </tr>
        </table>

<?php
        echo("<h3 align='left'>Update Search Confirmation</h3>");
        
        if($msg)                                             //if we have an error
        {
            error($msg);                                     //display it
            unset($msg);
        }
         
        echo("<form action='$PHP_SELF' method='post'>");
        echo("<input type='hidden' name='select' value='manage_searches' />");

        if($new_search_name)                                 //if the previous page is not the Save Search form
        {
            $temp_search_name = $new_search_name;            //then use the name from the "Rename Search" text field
        }
        else
        {
            $temp_search_name = $search_name;                //otherwise use the name from the Save Search Form
        }

        echo("<table border='0' width='100%' align='LEFT'>");
            echo("<tr class='row1'>");
                echo("<td>");
        
        //this instruct statement explains that we have a naming confict and asks user for a decision
        instruct("<center>
                      <p><br />The search name <b class=instruct>".$temp_search_name."</b> is already in use.</p>
                      <HR width='50%' size='1'>
                      <p>Do you wish to <b class=instruct>update (and overwrite)</b> the current search name?
                      <input type='submit' name='search_update_type' value='Update Search' /></p>
                      <p><b class='black'>- OR -</b></p>
                      <p>Do you want to save your search <b class=instruct>under a different name</b>?
                      <input type='text' name='new_search_name' /> <input type='submit' name='search_update_type' value='Rename Search' /><br /></p>
                      <p><b class='black'>- OR -</b></p>
                      <p>Do you wish to <b class=instruct>cancel (and discard the changes)</b> made to the search?
                      <input type='submit' name='search_update_type' value='Cancel' /></p>
                      <input type='hidden' name='continue' value='Check Update' />
                      <input type='hidden' name='select' value='manage_searches' />
                      <input type='hidden' name='search_name' value='$search_name' />
                      <input type='hidden' name='search' value='".$search."' />
                      <input type='hidden' name='search_id' value='$search_id' />
                      <input type='hidden' name='module_id' value='".$module_id."' />
                      <input type='hidden' name='search_description' value='".$search_description."' />
                  </center>");
     
                echo("</td>");
            echo("</tr>");
        echo("</table>");
        echo("</form>");        
    }
    elseif($continue == "Rename Search")
    {
        $no_tables = false;  //after displaying the search has been renamed we want to show the list of saved searches
        
        $new_search_name = strip_tags($new_search_name,'');
        if($search_description)
        {
            $search_description = strip_tags($search_description,'');
        }
        
        //create a new entry in the database using the same information as the old search only using the new name
        $sql = ("INSERT INTO search
                 (search, search_name, contact_id, search_module_id, search_description)
                 VALUES
                 (
                 '" . $search . "',
                 '" . $new_search_name . "',
                 '" . $auth->contact_id . "',
                 '" . $module_id . "',
                 '" . $search_description . "'
                 )");

        $result = $GLOBALS['dbh']->Execute($sql);
        
        $nmsg="'".removeSlashes($new_search_name)."' has been created.";                                           
    }
    elseif($continue == "Update Search")
    {
        $no_tables = false;  //after displaying the search has been updated  we want to show the list of saved searches
        
        //update the saved searches new information (if any)
        // If we are given a search object, than update it in the database. Otherwise, don't bother updating the search object. 
        if ($search && is_string($search))
        {
            $sql = ("UPDATE search
                     SET search_name='".$search_name."', search_description='".$search_description."', search = '".$search."', search_module_id = '".$module_id."'
                     WHERE search_id = '".$search_id."'");
        }

        else
        {
            $sql = ("UPDATE search
                     SET search_name='".$search_name."', search_description='".$search_description."', search_module_id = '".$module_id."'
                     WHERE search_id = '".$search_id."'");
        }

        $result = $GLOBALS['dbh']->Execute($sql);
        
        $nmsg="'".removeSlashes($search_name)."' has been updated.";
    }

}

// NO TABLES SECTION - this section displays the list of saved searches if $no_tables is false   
if(!$no_tables)
{    
?>

    </td>
    <td align="left">
        <img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" />
    </td>
    </tr>
    </table>

    <form action='<?php echo $PHP_SELF?>' method='post'>
        <input type='hidden' name='select' value='manage_searches' />

        <h3 align='left'>Manage Searches</h3>

<?php
    if($nmsg)
    {
        notify($nmsg);
    }
    
    if($emsg)
    {
        error($emsg);
    }
    
    //Find all Saved Searches (including their name, module, description and ID)
    $sql = "SELECT s.search_name, sm.module_name, s.search_description, s.search_id
            FROM search as s, search_module as sm
            WHERE s.contact_id = '".$auth->contact_id."'
            AND s.search_module_id = sm.module_id
            ORDER BY sm.module_name, s.search_name";

    $result = $GLOBALS['dbh']->Execute($sql);
    if($result->RecordCount() > 0)                //if we get results from our query
    {
?>

        <table cellspacing="0" cellpadding="10" border="0">
            <tr>
                <td colspan ='1' width="100%"  align='center'>
           
                    <table border=1 cellpadding=0 cellspacing=0>
                        <tr>
                            <td>
                               
                                <table border=0 cellpadding=2>
                                     <tr>
                                         <td class='rowgrey' align=center>&nbsp;</td>
                                         <td class='rowgrey' align=center><b class='white'>&nbsp;Name&nbsp;</b></td>
                                         <td class='rowgrey' align=center><b class='white'>&nbsp;Module&nbsp;</b></td>
                                         <td class='rowgrey' align=center><b class='white'>&nbsp;Search Description&nbsp;</b></td>
                                     </tr>

<?php
        //displaying the saved searches
        $rowclass = 1;
        while ($row = $result->FetchRow())
        {
            if($rowclass % 2 == 0)  //used to determine what color each row should be (we want alternating colors in our table)
            {
                $rclass = "row0d";  //even rows are of class "row0d"
            }
            else
            {
                $rclass = "row1d";  //odd rows are of class "row1d"
            }
            
            echo("<tr class='".$rclass."'>");
                echo("<td align='center'>&nbsp;&nbsp;<input type='radio' name='search_id' value='".$row["search_id"]."' />&nbsp;</td>");
                echo("<td align='center'>&nbsp;".wordwrap($row["search_name"], 50,"<br />")."</a>&nbsp;</td>\n");
                echo("<td align='center'>&nbsp;".$row["module_name"]."&nbsp;</td>\n");
                echo("<td align='left'>&nbsp;".wordwrap($row["search_description"], 200,"<br />")."&nbsp;</td>\n");
            echo("</tr>");

            $rowclass++;
        }
        
        echo("</table>");
        echo("</td>
              </tr>
              </table>");
                
        echo("</td>
              </tr>
              <tr>
                  <td align='left'>");
                      echo("<input type='submit' name='continue' value='Perform Search' />");
                      echo("&nbsp;&nbsp;<input type='submit' name='continue' value='Edit Search Name/Description' />");
                      echo("&nbsp;&nbsp;<input type='submit' name='continue' value='Edit Search Criteria' />");
                      echo("&nbsp;&nbsp;<input type='submit' name='continue' value='Delete Search' />
                  </td>
              </tr>
          </table>");
       echo(" </form>");
    }
    else
    {
        notify("There are no saved searches");
        echo(" </form>");
    }  
}   
?>
