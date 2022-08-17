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
 | Filename: mergesort.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

/*
 * mergesort.inc - sorting routine for arrays of arrays
 *
 * Exported to mergesort.inc 09/17/2001.
 * Code last modified 09/13/2001.
 */

/*
 * Sorts a list using merge sort, a recursive sorting algorithm that is THETA(n lg n)
 * (I think :^)
 *
 * listOfItems: an array of arrays of items
 * keyToSortBy: index of inner array to sort by
 * keyType: 0==string, 1==integer (NOT IMPLEMENTED), 99==last name (horrible hack!)
 */
function mergesort ($listOfItems, $keyToSortBy, $keyType)
{
  $length = count ($listOfItems);

  if ($length < 2) // Base case.
    return $listOfItems;

  // divide lists
  $left_list = $listOfItems;
  for ($i=0; $i<($length / 2); $i++)
    $right_list[] = array_shift ($left_list);

  // recursively sort
  $left_list_sorted = mergesort ($left_list, $keyToSortBy, $keyType);
  $right_list_sorted = mergesort ($right_list, $keyToSortBy, $keyType);

  // merge lists
  $sorted = array ();
  while (count ($left_list_sorted) > 0 || count ($right_list_sorted) > 0)
  {
    $compare_left = $left_list_sorted[0][$keyToSortBy];
    $compare_right = $right_list_sorted[0][$keyToSortBy];

    if ($keyType == 99)
    {
      $exploded_left = explode (" ", $compare_left);
      $compare_left = $exploded_left[1]." ".$exploded_left[0];
      $exploded_right = explode (" ", $compare_right);
      $compare_right = $exploded_right[1]." ".$exploded_right[0];
    }

    if ((count($right_list_sorted) == 0 || strcmp($compare_left, $compare_right) < 0) && (count($left_list_sorted) > 0))
      array_push($sorted, array_shift ($left_list_sorted));
    else
      array_push($sorted, array_shift ($right_list_sorted));
  }

  return $sorted;
}

