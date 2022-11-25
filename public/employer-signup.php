<?php
    $title = "Employer Sign up";
    require_once('../include/functions/main.php');
    require('../include/layout/header.php');
    logged_in_redirect();

    if (isset($_POST['register_employer'])) {

        $company_name =htmlentities(strip_tags(trim($_POST['company_name'])));
        $company_website =htmlentities(strip_tags(trim($_POST['company_website'])));
        $company_email =htmlentities(strip_tags(trim($_POST['company_email'])));
        $unhashed_company_password =htmlentities(strip_tags(trim($_POST['company_password'])));
        $company_password = password_hash($unhashed_company_password , PASSWORD_DEFAULT);
        $company_mobile =htmlentities(strip_tags(trim($_POST['company_mobile'])));
        $company_address =htmlentities(strip_tags(trim($_POST['company_address'])));
        $company_size =htmlentities(strip_tags(trim($_POST['company_size'])));
        $company_about =htmlentities(strip_tags(trim($_POST['company_about'])));
        $industry_type =htmlentities(strip_tags(trim($_POST['industry_type'])));
        $email_code = md5($industry_type . microtime());

            if (registerEmployer($company_name, $company_email, $company_website, $company_password, $company_mobile, $company_address, $company_size, $industry_type, $company_about, $email_code)) {

                echo "true";

            }else {

                echo "string";

            }


    }

 ?>

    <div class="container">
        <div style="margin:30px" class="sign-up-form">

          <img class="img-responsive center-block" src="img/call.png" alt="" />
          <form class="form-group employer-sign-up"  id="employer-sign-up" action="" method="post">

            <div class="col-sm-6">

              <div class="form-group">
                <label for=""><b>Company Name:</b></label>
                <input type="text" name="company_name" id="company_name" value="" class="form-control">
              </div>

              <div class="form-group">
                <label for=""><b>Company Website:</b></label>
                <input type="text" name="company_website" id="company_website" value="" class="form-control">
              </div>

              <div class="form-group">
                <label class="control-label"><b>Company Email:</b></label><small style="font-size:10px;color:red"><b> (Company email will be used for login) </b></small>
                <input type="text" name="company_email" id="company_email" value="" class="form-control">

              </div>


              <div class="form-group">
                <label for=""><b>Company mobile:</b></label>
                <input type="text" name="company_mobile" id="company_mobile" value="" class="form-control">
              </div>


            </div>

            <div class="col-sm-6 ">

                <div class="form-group">
                  <label for=""><b>Choose a Password for login:</b></label>
                  <input type="password" name="company_password"  id="company_password" value="" class="form-control">
                </div>

                <div class="form-group">
                  <label for=""><b>Confirm Password:</b></label>
                  <input type="password" name="confirm_password" id="confirm_password" value="" class="form-control">
                </div>

                <div class="form-group">
                  <label for=""><b>Company size:</b></label>
                  <select class="form-control" name="company_size" id="company_size">
                    <option value="option">option</option>
                  </select>
                </div>

                <div class="form-group">
                  <label for=""><b>Industry Type:</b></label>
                  <select name="industry_type" id="industry_type" class="form-control"><!-- Specialization Dropdown -->
                     <option value="option" selected="" disabled="">Select an option</option>
                     <option value="Accounting / Audit / Tax">Accounting / Audit / Tax</option>
                     <option value="Administration & Office Support">Administration &amp; Office Support</option>
                     <option value="Agriculture/Farming">Agriculture/Farming</option>
                     <option value="Banking / Finance / Insurance">Banking / Finance / Insurance</option>
                     <option value="Building Design/Architecture">Building Design/Architecture</option>
                     <option value="Construction">Construction</option>
                     <option value="Consulting/Business Strategy & Planning">Consulting/Business Strategy &amp; Planning</option>
                     <option value="Creatives(Arts, Design, Fashion)">Creatives(Arts, Design, Fashion)</option>
                     <option value="Customer Service">Customer Service</option>
                     <option value="Education/Teaching/Training">Education/Teaching/Training</option>
                     <option value="Engineering">Engineering</option>
                     <option value="Executive / Top Management">Executive / Top Management</option>
                     <option value="Healthcare / Pharmaceutical">Healthcare / Pharmaceutical</option>
                     <option value="Hospitality / Leisure / Travels">Hospitality / Leisure / Travels</option>
                     <option value="Human Resources">Human Resources</option>
                     <option value="Information Technology">Information Technology</option>
                     <option value="Legal">Legal</option>
                     <option value="Logistics / Transportation">Logistics / Transportation</option>
                     <option value="Manufacturing / Production">Manufacturing / Production</option>
                     <option value="Marketing / Advertising / Communications">Marketing / Advertising / Communications</option>
                     <option value="NGO/Community Services & Dev">NGO/Community Services &amp; Dev</option>
                     <option value="Oil&Gas / Mining / Energy">Oil&amp;Gas / Mining / Energy</option>
                     <option value="Project / Programme Management">Project / Programme Management </option>
                     <option value="QA&QC / HSE">QA&amp;QC / HSE</option>
                     <option value="Real Estate / Property">Real Estate / Property</option>
                     <option value="Research">Research</option>
                     <option value="Sales/Business Development">Sales/Business Development</option>
                     <option value="Supply Chain">Supply Chain / Procurement</option>
                     <option value="Telecommunications">Telecommunications</option>
                     <option value="Vocational Trade and Services">Vocational Trade and Services</option>
                     </select>
                </div>



            </div>

            <div class="col-sm-12">

              <div class="form-group">
                  <label for=""><b>Company address: </b></label>
                  <input type="text" name="company_address" id="company_address" value="" class="form-control">
              </div>

              <div class="form-group">
                <label for="firstname"><b>About the company</b></label>
                <textarea name="company_about" id="company_about" class="form-control" rows="10" cols="40"></textarea>
              </div>

              <button style="margin-bottom:20px;" type="submit" name="register_employer" id="register_employer" class="btn btn-primary pull-right">Register</button>

            </div>
          </form>
        </div>
    </div>



 <?php
   require('../include/layout/footer.php');
 ?>
