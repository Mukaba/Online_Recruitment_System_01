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
 | Filename: show_menu.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

/* This also works with a javascript menu, it does load faster than the one that is currently being used,
	however, it doesn't seem to work properly, so we're sticking with the other one.
*/

// check to see if the user wants to use javascript menu
// NOTE: This is where the global variables for the javascript menu are set
if ($userlevel == OFFICE) {

    $sql = "Select menu_use_javascript from contact_internal where contact_id='". $auth->contact_id ."'";
    $result = $GLOBALS['dbh']->Execute ($sql);
    $row = $result->FetchRow();

    $layer_count = 0; // tally for the layers
    $num_menu_items = 0; // total menu items to be displayed, so we can calculate placement of popups

    if ($row["menu_use_javascript"] == 1) 
        $has_java_enabled = 1;
    else
        $has_java_enabled = 0;
}

else {

    $has_java_enabled = 0;
}	

// this is used as a collection for the normal menu, incase the user selected using 
// javascript menu, but browser doesn't allow it
$submenu = "";

// default menu selected to interview module
if ($menu_select == "")
	$menu_select=10;
	
if (($select == "login") || ($select == "logout") || !$menu_select) {
	$jmenu_temp_off = 1;
}


switch ($userlevel) {

    case ADMINSTUDENT:
        $menu .= "<tr valign='top'><td><img src='".$GLOBALS['colorscheme']['bullet_menuitem']."' alt='' /></td><td valign='middle' class='menuitem' colspan='2'><font size='1' face='verdana, geneva, helvetica, arial'><a href='$PHP_SELF_MENU'>Back to Admin</a></font></td></tr>";
        // turn off Jmenu
        $jmenu_temp_off = 1;
        // drop through

    case STUDENT:
        $sql = "SELECT i.menu_name, i.menu_id, i.module_id
            FROM menu_item as i, department_module as d
            WHERE i.module_id = d.module_id 
            AND i.menu_owner = '" . STUDENT . "' 
            AND d.department_id = '" . addslashes($auth->department) . "'
            ORDER BY i.order_by";
        break;

    case OFFICE:
        $sql = "SELECT i.menu_name, i.menu_id, i.module_id
            FROM menu_item as i, department_module as d
            WHERE i.module_id = d.module_id 
            AND i.menu_owner = '" . OFFICE . "' 
            AND d.department_id= '" . addslashes($auth->department) . "'
            ORDER BY i.order_by";
        break;

    case FACULTY:
        $sql = "SELECT i.menu_name, i.menu_id, i.module_id
            FROM menu_item as i, department_module as d
            WHERE i.module_id = d.module_id 
            AND i.menu_owner = '" . FACULTY . "' 
            AND d.department_id= '" . addslashes($auth->department) . "'
            ORDER BY i.order_by";
        break;

    case TRAFFICDIRECTOR:
        $sql = "SELECT DISTINCT mi.menu_name, mi.menu_id, mi.module_id
            FROM menu_item AS mi, department_module AS dm
            WHERE mi.module_id=dm.module_id
            AND mi.menu_owner='" . TRAFFICDIRECTOR . "'
            AND dm.department_id='" . addslashes($auth->department) . "'
            ORDER BY mi.order_by";
        break;

    case EMPLOYER:
    case HREMPLOYER:
        $sql = "SELECT i.menu_name, i.menu_id, i.module_id
            FROM menu_item as i
            WHERE i.menu_owner = '" . $userlevel . "'
            ORDER BY i.order_by"; 
        break;

    case ADMINEMPLOYER:
        $menu .= "<tr valign='top'><td><img src='".$GLOBALS['colorscheme']['bullet_menuitem']."' alt='' /></td><td valign='middle' class='menuitem' colspan='2'><font size='1' face='verdana, geneva, helvetica, arial'><a href='$PHP_SELF_MENU'>Back to Admin</a></font></td></tr>";
        // turn off Jmenu
        $jmenu_temp_off = 1;
        $sql = "SELECT i.menu_name, i.menu_id, i.module_id
            FROM menu_item as i
            WHERE i.menu_owner = '" . EMPLOYER . "'
            ORDER BY i.order_by";
        break;

    default:
        $sql = "SELECT i.menu_name
            FROM menu_item AS i
            WHERE 1=2";
}

// make sure we actually have a query in order to query it
if ($sql) {
    $result = $GLOBALS['dbh']->Execute ($sql);
}

//esthetic purposes
$menu .= "<tr valign='top'><td><img src='".$GLOBALS['colorscheme']['bullet_menusubitem1']."' alt='' /></td><td valign='middle' class='menuitem' colspan='2'></td></tr>";
$subrownum=0;
echo("<script type='text/javascript'> var bgcolor='#CCCCCC'; </script>");
while ($row = $result->FetchRow()) {

    if ($row["menu_name"] == "Feedback") {
        
        $menu .= ("<tr valign='top'><td><img src='".$GLOBALS['colorscheme']['bullet_menuitem']."' alt='' /></td><td valign='middle' class='menuitem' colspan='2'><font size='1' face='verdana, geneva, helvetica, arial'><a href='$PHP_SELF_MENU?select=feedback");
        // throw in special stuff if signed in as someone else
        if ($userlevel == ADMINSTUDENT) {
            $menu .= ("&amp;admin_student_login=yes&amp;student_number_login=$student_number");
        }
        $menu .= ("'>" . $row["menu_name"] . "</a> </font> </td> </tr>");

    }

    elseif ($row["menu_name"] == "Logout") {
        
        $menu .= ("<tr valign='top'><td><img src='".$GLOBALS['colorscheme']['bullet_menuitem']."' alt='' /></td><td valign='middle' class='menuitem' colspan='2'><font size='1' face='verdana, geneva, helvetica, arial'><a href='$PHP_SELF_MENU?select=logout'>Logout</a></font></td></tr>");

    }

    else{
        
        // the actual menu that we need for the javascript menu
        $menu .= ("<tr valign='top'><td><img src='".$GLOBALS['colorscheme']['bullet_menuitem']."' alt='' /></td><td valign='middle' class='menuitem' colspan='2'>");
        $menu .= ("<font size='1' face='verdana, geneva, helvetica, arial'>");

        // if we're on the current menu item & we're using javascript menues, make it bold so it stands out
        if (($menu_select == $row["menu_id"]) && ($has_java_enabled && !$jmenu_temp_off)) {
            $menu .= "<b>";
        }

        // start href stuff
        $menu .= ("<a href=\"$PHP_SELF_MENU?menu_select=" . urlencode($row["menu_id"]));

        // throw in special stuff if signed in as someone else
        if ($userlevel == ADMINSTUDENT) {
            $menu .= ("&amp;admin_student_login=yes&amp;student_number_login=$student_number");
        }
        elseif ($userlevel == ADMINEMPLOYER) {
            $menu .= ("&amp;admin_employer_login=yes&amp;employer_contact_id_login=$employer_contact_id_login");
        }

        // close off the quotes from the href
        $menu .= "\"";

        // if they have it enabled, throw in a variable for javascript
        if ($has_java_enabled && !$jmenu_temp_off) {
            $menu .= ("id='Menu$layer_count' onmouseover=\"if (Submenu$layer_count != '') return overlib(Submenu$layer_count, STICKY, MOUSEOFF, LEFT, FIXY, getElementTop('Menu$layer_count') + 4, WIDTH, 100, VAUTO, FIXX, (getElementLeft('Menu$layer_count') + getElementWidth('Menu$layer_count') + 4) , TIMEOUT, 3000)\" onmouseout=\"nd();\" ");
        }

        // close off tag for opening href & put name and closing href
        $menu .= (">". $row["menu_name"] . "</a>");

        // if we're on the current menu item & we're using javascript menues, make it bold so it stands out (closing tag)
        if (($menu_select == $row["menu_id"]) && ($has_java_enabled && !$jmenu_temp_off)) {
            $menu .= "</b>";
        }

        $menu .= ("</font></td></tr>");


        // this nice little thing deals with the sub menu for the Jmenu
        if ($has_java_enabled && !$jmenu_temp_off) {

            $sub_sql = "SELECT menu_name, select_var
                FROM menu_sub_item
                WHERE menu_id = '" . $row["menu_id"] . "'
                ORDER BY order_by";

            $sub_result = $GLOBALS['dbh']->Execute ($sub_sql);
            $num_submenus = $sub_result->RecordCount();
            $num_menu_items ++;

            if ($sub_result && ($num_submenus > 1)) {
/*
                // special circumstance
                if ($_SESSION['SESSION_colorscheme'] == 'uvic') {
                    $left = 100;
                }
                else {
                    $left = 120;
                }
                if (($client->platform == LINUX) || ($client->platform == UNIX)) {
                    $width = 150;
                }
                else {
                    $width = 110;
                }
                $top = $num_menu_items * 13;
                $jsubmenu_items = "";
                $width = 10;
*/
                $jsubmenu_items = "";
                while ($sub_row = $sub_result->FetchRow()) {

                    // here we've got a nifty little thing that will take the length of the name,
                    // if it's greater than what the length already is, then make it that length.
                    // this ensures that the width of the sub menu is as big as the biggest name
/*                    $name_length = strlen ($sub_row["menu_name"]);
                    if (($client->platform == LINUX) || ($client->platform == UNIX)) {
                        $name_length *= 8;
                    }
                    else {
                        $name_length *= 5;
                    }
                    if ($name_length > $width) {
                        $width = $name_length;
                    }
*/                  $subrownum++;
                    $jsubmenu_items .= "<tr width=\"100%\"><td width=\"100%\" style=\"cursor: hand;\" ";
                    $jsubmenu_items .= " onmouseover=\"this.style.background=bgcolor;\" ";
                    $jsubmenu_items .= " onmouseout =\"this.style.background=\'\' \"";
                    $jsubmenu_items .= " onclick=\"document.location=\'$PHP_SELF_MENU?menu_select=" . urlencode($row["menu_id"]) . "&amp;" . $sub_row["select_var"] . "\'\">";
                    $jsubmenu_items .= "<font size=\"1\" face=\"verdana, geneva, helvetica, arial\">";
                    $jsubmenu_items .= "<a href=\"$PHP_SELF_MENU?menu_select=" . urlencode($row["menu_id"]) . "&amp;" . $sub_row["select_var"] . "\">";
                    $jsubmenu_items .= "&nbsp;". ereg_replace(" ", "&nbsp;", $sub_row["menu_name"]) ."</a></font></td></tr>";
                }
                $jsubmenu .= "var Submenu$layer_count = '<table width=\"100%\">$jsubmenu_items</table>';\n";
            } else {
                $jsubmenu .= "var Submenu$layer_count = '';\n";
            }
            $layer_count ++;
        }
    }


    // this whole thing is the part that actually sets up the actual menu
    if ($menu_select == $row["menu_id"]) {
        
        $sql = "SELECT menu_name, select_var
            FROM menu_sub_item
            WHERE menu_id = '" . $row["menu_id"] . "'
            ORDER BY order_by";
        $sub_result = $GLOBALS['dbh']->Execute($sql);

        $num_rows = $sub_result->RecordCount();


        if ($sub_result && $num_rows > 0) {

            $sub_row = $sub_result->FetchRow();


            // This is where $select gets changed if no sub_menu is selected but a menu is selected.
            // Basically if the main menu item is selected it will open the page of the first sub item
            if ($select == "welcome" || $select == "") {

                $var_pairs = explode("&", $sub_row["select_var"]);
                for($i=0;$i<sizeof($var_pairs);$i++) {
                    list($var, $values) = explode("=", $var_pairs[$i]);
                    $$var = $values;
                }
            }

        
            /*
             If the user is a student, employer, or adminstudent, and the menu item has only one sub-item
             then do NOT display any sub items underneath the parent item.  Likewise if the
             user is an administrator or office user and they're clicking on the help menu item.
            */
        
            if (
            (!(($userlevel == OFFICE && $row["menu_name"] == 'Help') || ($userlevel == ADMINISTRATOR && $row["menu_name"] == 'Help') 
            )) && !($num_rows <= 1 && ($userlevel == ADMINSTUDENT || $userlevel == STUDENT || $userlevel==EMPLOYER 
            || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER || $userlevel == FACULTY))) {

        
                $submenu .= ("<tr class='menuitem'>");
                $submenu .= ("<td>");
                   $submenu .= ("<img src='".$GLOBALS['colorscheme']['bullet_menusubitem1']."' alt='' />");
                $submenu .= ("</td>");
                $submenu .= ("<td>");
                   $submenu .= ("<img src='".$GLOBALS['colorscheme']['bullet_menusubitem2']."' alt='' />");
                $submenu .= ("</td>");
                $submenu .= ("<td class='menuitem'>");
                   $submenu .= ("<font size='1' face='verdana, geneva, helvetica, arial'>");
                   $submenu .= ("<a href='$PHP_SELF_MENU?menu_select=" . urlencode($row["menu_id"]));

                   if ($userlevel == ADMINSTUDENT)
                   {
                       $submenu .= ("&amp;admin_student_login=yes&amp;student_number_login=$student_number");
                   }
                   elseif ($userlevel == ADMINEMPLOYER)
                   {
                       $submenu .= ("&amp;admin_employer_login=yes&amp;employer_contact_id_login=$employer_contact_id_login");
                   }
                
                   $submenu .= ("&amp;" . $sub_row["select_var"] ."'>" . $sub_row["menu_name"] . "</a>");
                $submenu .= ("</font></td>");
                $submenu .= ("</tr>\n");
                  

                while ($sub_row = $sub_result->FetchRow()) {
                
                    $submenu .= ("<tr><td class='menuitem'><img src='".$GLOBALS['colorscheme']['bullet_menusubitem1']."' alt='' /></td><td class='menuitem'><img src='".$GLOBALS['colorscheme']['bullet_menusubitem2']."' alt='' /></td><td class='menuitem'><font size='1' face='verdana, geneva, helvetica, arial'><a href='$PHP_SELF_MENU?menu_select=" . urlencode($row["menu_id"]));

                    if ($userlevel == ADMINSTUDENT) 
                        $submenu .= ("&amp;admin_student_login=yes&amp;student_number_login=$student_number");
                    elseif ($userlevel == ADMINEMPLOYER)
                        $submenu .= ("&amp;admin_employer_login=yes&amp;employer_contact_id_login=$employer_contact_id_login");

                    $submenu .= ("&amp;" . $sub_row["select_var"] . "'");
                    
                    $submenu .= ">" . $sub_row["menu_name"] . "</a></font></td></tr>\n";
                }
            } 
        }
    }

    // repository for the menus, one for jmenu, one just in case, then reset for the next pass
    $jmenu .= $menu;
    $non_jmenu .= $menu . $submenu;
    $menu = "";
    $submenu = "";
}

// repository for the menus, one for jmenu, one just in case, then reset for the next pass
// this is extra just in case we don't go through the loop at all above (ie: login screen)
$jmenu .= $menu;
$non_jmenu .= $menu . $submenu;
$menu = "";
$submenu = "";

$jmenu = "<table cellpadding='0' cellspacing='0' border='0'>$jmenu</table>";
$non_jmenu = "<table cellpadding='0' cellspacing='0' border='0'>$non_jmenu</table>";

if ($has_java_enabled && !$jmenu_temp_off) {
 echo("\n<script type=\"text/javascript\" language=\"javascript\">\n//<![CDATA[\n".$jsubmenu ."\n//]]>\n</script>\n");
  
   
echo('<script src="misc/js_pos_api.js" type="text/javascript"></script>');
   // in case their browser doesn't support it, but they want it
   echo ("\n<script type=\"text/javascript\">\n//<![CDATA[\ndocument.write(\"". addslashes ($jmenu) ."\")\n//]]>\n</script>\n");
   echo ("\n<noscript>\n$non_jmenu\n</noscript>\n");

}

else {

    echo $non_jmenu;
}

?>
