<template>
  <v-switch
    dense
    hide-details
    class="ma-1 py-0"
    :multiple="multiple"
    :label="label"
    :value="value"
    :color="color"
    :input-value="inputValue"
    @change="$emit('change', $event)"
  />
</template>

<script>
export default {
  name: "mod-checkbox",

  model: {
    prop: "inputValue",
    event: "change"
  },

  props: {
    inputValue: [Boolean, Array],
    multiple: {
      type: Boolean,
      default: null
    },
    label: String,
    value: String,
    color: String
  }
};
</script>
