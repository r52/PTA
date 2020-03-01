<template>
  <v-text-field
    single-line
    dense
    hide-details
    type="number"
    clearable
    v-model.number="filter[type]"
    v-on:input="filter.enabled = true"
    :disabled="disabled"
    @keypress="isNumber($event)"
    class="my-1 py-0"
  />
</template>

<script>
export default {
  name: "ModNumInput",

  props: ["filter", "type", "settings", "disabled"],

  methods: {
    isNumber: function(evt) {
      evt = evt ? evt : window.event;
      var charCode = evt.which ? evt.which : evt.keyCode;
      if (
        charCode > 31 &&
        (charCode < 48 || charCode > 57) &&
        charCode !== 46 &&
        charCode !== 45
      ) {
        evt.preventDefault();
      } else {
        return true;
      }
    }
  }
};
</script>
