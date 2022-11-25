<?php

require '../database/pdo_connect.php';

$filter = htmlentities(strip_tags(trim($_POST['search'])));

$query = $pdo->prepare("SELECT * FROM jobs WHERE title LIKE '$filter%' ORDER BY date_posted ASC" );

    if ($query->execute()) {

      if ($jobs = $query->fetchAll(PDO::FETCH_ASSOC)) {

          // echo "true";
          ?>
          <table class="table table-striped">
              <thead>
                  <tr>
                    <th>S/no</th>
                    <th>Company Name</th>
                    <th>Job Title</th>
                    <th>Date posted</th>
                    <th>Application Deadline</th>
                    <th>Qualification</th>
                    <th>Gender</th>
                    <th>Status</th>
                    <th></th>
                  </tr>
              </thead>
              <tbody>

                    <?php
                            $i = 1;
                            foreach ($jobs  as $key => $jobs) {
                    ?>

                            <tr>
                              <td><?=$i;?></td>
                              <td><?=$jobs['employer']?></td>
                              <td><?=$jobs['title']?></td>
                              <td><?=$jobs['date_posted']?></td>
                              <td><?=$jobs['deadline']?></td>
                              <td><?=$jobs['qualification']?></td>
                              <td><?=$jobs['gender']?></td>
                              <td><?=$jobs['status']?></td>
                              <td><a href="apply.php?job=<?=$jobs['id']?>">Apply</a></td>
                            </tr>

                    <?php
                            $i++;
                            }
                     ?>

            </tbody>
          </table>

          <?php

      }
      else { ?>

        <table class="table table-striped">
            <thead>
                <tr>
                  <th>S/no</th>
                  <th>Company Name</th>
                  <th>Job Title</th>
                  <th>Date posted</th>
                  <th>Application Deadline</th>
                  <th>Qualification</th>
                  <th>Gender</th>
                  <th>Status</th>
                  <th></th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                      No result was found
                    </td>
                </tr>
            </tbody>
        </table>


      <?php
      }

    }

 ?>
