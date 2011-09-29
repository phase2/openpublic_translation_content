<?php

/*
 * implements hook_apps_app_info
 */
function openpublic_translation_content_apps_app_info() {
  return array(
    'configure form' => 'openpublic_translation_content_configure',
  );
}

/*
 * configure form for openpublic translation content app
 */
function openpublic_translation_content_configure($form, $form_state) {
  $form = array();

  # Point to Langauge page
  $form['language'] = array(
    '#type' => 'fieldset',
    '#title' => 'Drupal Language Settings',
  );

  $langs = array();
  foreach(language_list() as $code => $lang) {
    $langs[$code] = $lang->name;
  }
  $lang_last = array_pop($langs);
  $langs_string = implode(" and ", array(implode(', ', $langs), $lang_last));
  $form['language']['markup'] = array(
    '#type' => 'markup',
    '#markup' => t("Currently the site in enabled for @langs, to administer this or add more please see the !language_config_link.", array('@langs' => $langs_string, '!language_config_link'=> l('Language Configuration Page', 'admin/config/regional/language'))),
  );
  # if we only have one language lets warn the user
  if(empty($langs)) {
    $form['language']['markup']['#markup'] = t('Currently only one language is configured please add additional languages on the !language_config_link.', array('!language_config_link'=> l('Language Configuration Page', 'admin/config/regional/language')));

    $form['language']['markup']['#prefix'] = "<div class = 'error'>";
    $form['language']['markup']['#suffix'] = "</div>";
  }
  # Point to Translation page
  $form['translation'] = array(
    '#type' => 'fieldset',
    '#title' => 'Drupal Translation Interface',
  );
  $form['translation']['markup'] = array(
    '#type' => 'markup',
    '#markup' => t('Drupal Provides an interface to translate the administion pages on the  !language_config_link.', array('!language_config_link'=> l('Translation Inteface Page', 'admin/config/regional/translate'))),
  );

  #allow for views to not get alter with a language filter
  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => 'Langauge View Alters',
  );
  $views = array();
  foreach(views_get_enabled_views() as $view) {
    if($view->base_table =='node') {
      $views[$view->name] = $view->human_name ? $view->human_name : $view->name;
    }
  }
  $views_selected = variable_get('openpublic_translation_content_excluded_views', array());
  $form['views']['views_excluded'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Views that should Not be Alter to filter on Language',
    '#description' => t('OpenPublic Translation Content will add a filter to all views to filter on the current language, any views selected here will not have that filter added.'),
    '#options' => $views,
    '#default_value' => $views_selected,
  );
  $form['sumbit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;
}
/*
 * implement submit for openpublic_translation_content_configure form
 */
function openpublic_translation_content_configure_submit($form, &$form_state) {
  #save value of excluded views
  variable_set('openpublic_translation_content_excluded_views', $form_state['values']['views_excluded']);
}
