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
 | Filename: scheme.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/
function scheme_sort(&$a, &$b){
    if ($a->getName() == $b->getName()) {
        return 0;
    }
    return ($a->getName() < $b->getName()) ? -1 : 1;
}

$arr_schemes = get_hooks("colorscheme.get");
usort($arr_schemes, "scheme_sort");
?>
<!-- Beginning of scheme.inc --><h3>Preferences - Personal: Color Scheme</h3>

<table width="75%" cellpadding="5" cellspacing="2" border="0">
<?php if (count($arr_schemes) == 0 || ALLOW_GENERIC_COLORSCHEME){ ?>
<tr><td class="row1" nowrap="nowrap"><a class="black" href="mamook.php?menu_select=23&amp;colorscheme=none&amp;select=prefer">Mamook Default Scheme:</a></td>
	<td class="rowgrey"><a class='preference' href="mamook.php?menu_select=23&amp;colorscheme=none&amp;select=prefer">Change</a></td>
	<td><?php if ($_SESSION['SESSION_colorscheme'] == "none") echo "<b class=\"blue\">current</b>"; ?></td>
</tr>
<?php
}
foreach ($arr_schemes as $scheme){
    print "<tr><td class=\"row1\" nowrap=\"nowrap\"><a class=\"black\" href=\"mamook.php?menu_select=23&amp;colorscheme=".$scheme->getIdentifier()."&amp;select=prefer\">".$scheme->getName().":</a></td>
	<td class=\"rowgrey\"><a class='preference' href=\"mamook.php?menu_select=23&amp;colorscheme=".$scheme->getIdentifier()."&amp;select=prefer\">Change</a></td><td>";
    if ($_SESSION['SESSION_colorscheme'] == $scheme->getIdentifier()) print "<b class=\"blue\">current</b>"; 
    print "</td></tr>";
}
?>
</table>

<!-- End of Prefs -->
