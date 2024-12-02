import yaml

CONFIG_FILE = "parameterConfig.yml"


def get_permutations_recurse(choices, index):
    if index == len(choices) - 1:
        return [
            {choices[index]["name"]: choice} for choice in choices[index]["choices"]
        ]

    permutations = []

    for choice in choices[index]["choices"]:
        deeper_params = get_permutations_recurse(choices, index + 1)
        chosen_param = {choices[index]["name"]: choice}
        for deeper_param in deeper_params:
            permutations.append({**chosen_param, **deeper_param})

    return permutations


def get_parameter_permutations(parameters):
    choices = [
        {"name": param, "choices": choice_list}
        for param, choice_list in parameters.items()
    ]

    permutations = get_permutations_recurse(choices, 0)
    return permutations


def generate_experiment_name(name_template, parameters):
    for key, value in parameters.items():
        name_template = name_template.replace(f"<{key}>", str(value))

    return name_template


def generate_param_list(name_template, num_replicates, parameters, positions, exe):
    experiment_name_base = generate_experiment_name(name_template, parameters)

    replicates = []
    for replicate in range(0, num_replicates):
        rep = [exe]
        parameters["experiment_name"] = f"{experiment_name_base}_{replicate}"

        for param_position in positions:
            if param_position in parameters:
                rep.append(f"'{parameters[param_position]}'")
            else:
                rep.append("")

        replicates.append(rep)

    return replicates


def permutate_experiment(
    parameters, num_replicates, experiment_name_template, positions, exe
):
    parameter_permutations = get_parameter_permutations(parameters)

    replicates = []
    for parameter_set in parameter_permutations:
        replicates.extend(
            generate_param_list(
                experiment_name_template, num_replicates, parameter_set, positions, exe
            )
        )

    return replicates


def permutate_experiments(permutation_config, positions, exe):
    experiment_name_template = permutation_config["experiment_name"]
    num_replicates = permutation_config["replicates"]

    parameters = permutation_config["parameters"]
    if type(parameters) is dict:
        parameters = [parameters]

    replicates = []
    for parameter_set in parameters:
        replicates.extend(
            permutate_experiment(
                parameter_set, num_replicates, experiment_name_template, positions, exe
            )
        )

    return replicates


def main():
    with open(CONFIG_FILE, "r") as f:
        config = yaml.safe_load(f)

    positions = config["positions"]
    exe = config["executable"]

    replicates = []
    if config["debug"]:
        replicates.append(["#", "exe", *positions])
    if "permutate_experiments" in config:
        replicates.extend(
            permutate_experiments(config["permutate_experiments"], positions, exe)
        )

    if "fixed_experiments" in config:
        for experiment in config["fixed_experiments"]:
            replicates.extend(
                generate_param_list(
                    experiment["experiment_name"],
                    experiment["replicates"],
                    experiment["parameters"],
                    positions,
                    exe,
                )
            )

    output(replicates, config["output_file"], config["debug"])


def output(replicates, output_file, out_console=False):
    replicates_string = "\n".join([" ".join(r) for r in replicates]) + "\n"
    if out_console:
        print(replicates_string)
        return

    with open(output_file, "w") as f:
        f.write(replicates_string)


if __name__ == "__main__":
    main()
    exit()
