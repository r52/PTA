<template>
  <v-row align="center" align-content="center" justify="space-between" no-gutters>
    <v-col cols="6">
      <v-switch
        dense
        hide-details
        class="my-1 py-0 magic--text"
        v-model.lazy="filter.enabled"
        :label="`(${filter.type}) ${filter.text}`"
      />
    </v-col>
    <v-col cols="1">
      <mod-num-input
        type="min"
        :filter="filter"
        :disabled="filter.value.length < 1 || filter.value.length > 2"
      />
    </v-col>
    <v-col cols="1" align-self="center">
      <p v-if="filter.value.length" class="text-center my-0 py-0">{{ filter.value[0] }}</p>
      <p v-else class="text-center my-0 py-0">N/A</p>
    </v-col>
    <v-col cols="1">
      <mod-num-input
        type="max"
        :filter="filter"
        :disabled="filter.value.length < 1 || filter.value.length > 2"
      />
    </v-col>
  </v-row>
</template>

<script>
import ModNumInput from "./ModNumInput.vue";

export default {
  name: "ModFilter",

  components: {
    ModNumInput
  },

  props: ["filter", "settings", "type"],

  created() {
    if (this.filter.value.length) {
      let range = this.settings.prefillrange / 100.0;
      let value = this.filter.value[0];
      let diff = range * value;

      if (this.settings.prefillmin) {
        this.filter["min"] = value - diff;
      }

      if (this.settings.prefillmax) {
        this.filter["max"] = value + diff;
      }
    }

    if (
      (this.type == "normal" && this.settings.prefillnormals) ||
      (this.type == "pseudo" && this.settings.prefillpseudos)
    ) {
      this.filter.enabled = true;
    }
  }
};
</script>

<style scoped>
.magic--text /deep/ label {
  color: #88f;
}
</style>
